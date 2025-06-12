package com.example.rgz;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Alert.AlertType;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class NewsEditController {

    @FXML
    private TextField titleField;
    @FXML
    private TextArea contentArea;

    private String originalTitle;

    private Runnable onNewsSaved;

    public void setOnNewsSaved(Runnable onNewsSaved) {
        this.onNewsSaved = onNewsSaved;
    }

    public void loadNewsData(String title) {
        this.originalTitle = title;
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword())) {
            String query = "SELECT title, news_content FROM news WHERE title = ?";
            try (PreparedStatement stmt = conn.prepareStatement(query)) {
                stmt.setString(1, title);
                ResultSet rs = stmt.executeQuery();
                if (rs.next()) {
                    titleField.setText(rs.getString("title"));
                    contentArea.setText(rs.getString("news_content"));
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            showError("Ошибка при загрузке новости.");
        }
    }

    @FXML
    private void handleSaveNews() {//обнова таблицы
        String newTitle = titleField.getText().trim();
        String newContent = contentArea.getText().trim();

        if (newTitle.isEmpty() || newContent.isEmpty()) {
            showError("Заполните все поля!");
            return;
        }

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword())) {
            String query = "UPDATE news SET title = ?, news_content = ?, updated_at = NOW() WHERE title = ?";
            try (PreparedStatement stmt = conn.prepareStatement(query)) {
                stmt.setString(1, newTitle);
                stmt.setString(2, newContent);
                stmt.setString(3, originalTitle);
                stmt.executeUpdate();
            }

            showInfo("Новость обновлена успешно!");

            if (onNewsSaved != null) {
                onNewsSaved.run();
            }

            closeWindow();
        } catch (Exception e) {
            e.printStackTrace();
            showError("Ошибка при сохранении изменений.");
        }
    }

    @FXML
    private void handleCancel() {
        closeWindow();
    }

    private void closeWindow() {
        Stage stage = (Stage) titleField.getScene().getWindow();
        stage.close();
    }

    private void showError(String message) {
        Alert alert = new Alert(AlertType.ERROR);
        alert.setTitle("Ошибка");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
    private void showInfo(String message) {
        Alert alert = new Alert(AlertType.INFORMATION);
        alert.setTitle("Информация");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }
}
