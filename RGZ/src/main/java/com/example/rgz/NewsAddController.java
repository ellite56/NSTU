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

public class NewsAddController {

    private Runnable onNewsSaved;

    @FXML
    private TextField titleField;

    @FXML
    private TextArea contentArea;

    @FXML
    private void handleSaveNews() {
        String title = titleField.getText().trim();
        String content = contentArea.getText().trim();

        if (title.isEmpty() || content.isEmpty()) {
            showError("Заполните все поля!");
            return;
        }

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword())) {
            String query = "INSERT INTO news (title, news_content, created_at, updated_at) VALUES (?, ?, NOW(), NOW())";//now() возвращает текущ дат и время
            try (PreparedStatement stmt = conn.prepareStatement(query)) {
                stmt.setString(1, title);
                stmt.setString(2, content);
                stmt.executeUpdate();
            }

            if (onNewsSaved != null) {
                onNewsSaved.run();
            }

            showInfo("Новость добавлена успешно!");
            closeWindow();
        } catch (Exception e) {
            e.printStackTrace();
            showError("Ошибка при сохранении новости.");
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
    public void setOnNewsSaved(Runnable onNewsSaved) {
        this.onNewsSaved = onNewsSaved;
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
