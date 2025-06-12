package com.example.rgz;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.ButtonType;
import javafx.scene.control.ListView;
import javafx.scene.layout.StackPane;
import javafx.scene.text.Text;
import javafx.scene.text.TextFlow;
import javafx.stage.Stage;
import javafx.scene.Scene;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.util.HashMap;
import java.util.Map;

public class NewsAdmin {

    @FXML
    private Button addNewsButton;
    @FXML
    private Button editNewsButton;
    @FXML
    private Button deleteNewsButton;
    @FXML
    private ListView<String> newsListView;
    @FXML
    private TextFlow newsContentArea;

    private Map<String, News> newsData = new HashMap<>();

    @FXML
    public void initialize() {
        addNewsButton.setOnAction(event -> handleAddNews());
        editNewsButton.setOnAction(event -> handleEditNews());
        deleteNewsButton.setOnAction(event -> handleDeleteNews());

        newsListView.getSelectionModel().selectedItemProperty().addListener((observable, oldValue, newValue) -> {
            if (newValue != null) {
                displayNewsContent(newValue);
            }
        });

        loadNewsData();
    }

    private void loadNewsData() {
        newsListView.getItems().clear();
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement stmt = conn.prepareStatement("SELECT title, news_content, created_at, updated_at FROM news");
             ResultSet rs = stmt.executeQuery()) {

            while (rs.next()) {
                String title = rs.getString("title");
                String content = rs.getString("news_content");
                String createdAt = rs.getString("created_at");
                String updatedAt = rs.getString("updated_at");

                newsData.put(title, new News(title, content, createdAt, updatedAt));

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

    @FXML
    private void handleAddNews() {
        openAddNewsWindow();
    }

    private void openAddNewsWindow() {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("add_news.fxml"));
            Parent root = loader.load();

            NewsAddController addController = loader.getController();
            addController.setOnNewsSaved(() -> {
                loadNewsData();
            });

            Stage stage = new Stage();
            stage.setTitle("Добавление новости");
            stage.setScene(new Scene(root, 400, 400));

            stage.show();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Ошибка при открытии окна добавления новости");
        }
    }

    @FXML
    private void handleEditNews() {
        String selectedTitle = newsListView.getSelectionModel().getSelectedItem();
        if (selectedTitle != null) {
            openEditNewsWindow(selectedTitle);
        }
    }

    private void openEditNewsWindow(String title) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("edit_news.fxml"));
            Parent root = loader.load();

            NewsEditController editController = loader.getController();
            editController.loadNewsData(title);

            editController.setOnNewsSaved(() -> {
                loadNewsData();
            });

            Stage stage = new Stage();
            stage.setTitle("Редактирование новости");
            stage.setScene(new Scene(root, 400, 400));

            stage.show();
        } catch (Exception e) {
            e.printStackTrace();
            System.out.println("Ошибка при открытии окна редактирования новости");
        }
    }

    @FXML
    private void handleDeleteNews() {
        String selectedTitle = newsListView.getSelectionModel().getSelectedItem();
        if (selectedTitle == null) {
            showError("Пожалуйста, выберите новость для удаления.");
            return;
        }

        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Подтверждение удаления");
        alert.setHeaderText("Вы уверены, что хотите удалить эту новость?");
        alert.setContentText("Новость будет удалена навсегда.");

        alert.showAndWait().ifPresent(response -> {
            if (response == ButtonType.OK) {
                deleteNews(selectedTitle);
            }
        });
    }

    private void deleteNews(String title) {
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword())) {
            String query = "DELETE FROM news WHERE title = ?";
            try (PreparedStatement stmt = conn.prepareStatement(query)) {
                stmt.setString(1, title);
                int rowsAffected = stmt.executeUpdate();

                if (rowsAffected > 0) {
                    showInfo("Новость успешно удалена.");
                    loadNewsData();
                } else {
                    showError("Не удалось удалить новость. Она может не существовать.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            showError("Ошибка при удалении новости.");
        }
    }

    private void showError(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Ошибка");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    private void showInfo(String message) {
        Alert alert = new Alert(Alert.AlertType.INFORMATION);
        alert.setTitle("Информация");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
}
