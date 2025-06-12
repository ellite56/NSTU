package com.example.rgz;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ListView;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import javafx.stage.Stage;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;

public class NewsUsers {

    @FXML
    private Button addCommentButton;

    @FXML
    private ListView<String> newsListView;

    @FXML
    private TextFlow newsContentArea;

    @FXML
    private ListView<String> commentsListView;

    private Map<String, News> newsData = new HashMap<>();

    @FXML
    public void initialize() {
        addCommentButton.setOnAction(event -> handleAddComment());
        newsListView.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue != null) {
                displayNewsContent(newValue);
                loadCommentsForNews(newValue);
            }
        });
        loadNewsData();
    }

    private void handleAddComment() {
        if (Login.isGuest()) {
            showLoginPrompt();
            return;
        }

        String selectedNewsTitle = newsListView.getSelectionModel().getSelectedItem();
        if (selectedNewsTitle == null) {
            showAlert("Ошибка", "Выберите новость", "Пожалуйста, выберите новость, чтобы добавить комментарий.");
            return;
        }

        openAddCommentWindow(selectedNewsTitle);
    }

    private void showLoginPrompt() {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Требуется вход");
        alert.setHeaderText("Комментарии доступны только для зарегистрированных пользователей.");
        alert.setContentText("Пожалуйста, войдите или зарегистрируйтесь, чтобы оставить комментарий.");
        alert.showAndWait();
    }

    private void openAddCommentWindow(String newsTitle) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("add_comment.fxml"));
            Parent root = loader.load();

            AddCommentController commentController = loader.getController();
            commentController.setNewsTitle(newsTitle);
            commentController.setOnCommentSaved(() -> {
                loadCommentsForNews(newsTitle);
            });

            Stage stage = new Stage();
            stage.setTitle("Добавить комментарий");
            stage.setScene(new Scene(root));
            stage.show();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void loadNewsData() {
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement stmt = conn.prepareStatement("SELECT id, title, news_content, created_at, updated_at FROM news");
             ResultSet rs = stmt.executeQuery()) {

            newsListView.getItems().clear();
            newsData.clear();

            while (rs.next()) {
                int id = rs.getInt("id");
                String title = rs.getString("title");
                String content = rs.getString("news_content");
                String createdAt = rs.getString("created_at");
                String updatedAt = rs.getString("updated_at");

                News news = new News(title, content, createdAt, updatedAt);
                news.setId(id);

                newsData.put(title, news);
                newsListView.getItems().add(title);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void displayNewsContent(String title) {
        News news = newsData.get(title);

        if (news == null) {
            newsContentArea.getChildren().clear();
            newsContentArea.getChildren().add(new Text("Содержимое не найдено."));
            return;
        }

        Text titleText = new Text(news.getTitle() + "\n\n");
        titleText.setStyle("-fx-font-weight: bold; -fx-font-size: 14px;");

        Text contentText = new Text(news.getContent() + "\n\n");

        Text createdAtText = new Text("Создано: " + news.getCreatedAt() + "\n");
        createdAtText.setStyle("-fx-font-size: 12px; -fx-fill: grey;");

        Text updatedAtText = new Text("Обновлено: " + news.getUpdatedAt());
        updatedAtText.setStyle("-fx-font-size: 12px; -fx-fill: grey;");

        newsContentArea.getChildren().clear();
        newsContentArea.getChildren().addAll(titleText, contentText, createdAtText, updatedAtText);
    }

    private void loadCommentsForNews(String newsTitle) {
        News news = newsData.get(newsTitle);
        if (news == null) return;

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement stmt = conn.prepareStatement(
                     "SELECT c.content, u.username, c.created_at " +
                             "FROM comments c " +
                             "LEFT JOIN users u ON c.user_id = u.id " +
                             "WHERE c.news_id = ?")) {

            stmt.setInt(1, news.getId());

            try (ResultSet rs = stmt.executeQuery()) {
                commentsListView.getItems().clear();
                while (rs.next()) {
                    String username = rs.getString("username");
                    if (username == null) username = "Гость";

                    String content = rs.getString("content");
                    String createdAt = rs.getString("created_at");

                    String commentDisplay = String.format("[%s] %s: %s", createdAt, username, content);
                    commentsListView.getItems().add(commentDisplay);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void showAlert(String title, String headerText, String contentText) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle(title);
        alert.setHeaderText(headerText);
        alert.setContentText(contentText);
        alert.showAndWait();
    }
}
