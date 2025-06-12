package com.example.rgz;

import javafx.fxml.FXML;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.TextArea;
import javafx.stage.Stage;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class AddCommentController {

    @FXML
    private TextArea commentField;

    @FXML
    private Button saveButton;
    @FXML
    private Button cancelButton;
    private String newsTitle;
    private Runnable onCommentSaved;
    public void setNewsTitle(String title) {
        this.newsTitle = title;
    }
    public void setOnCommentSaved(Runnable onCommentSaved) {
        this.onCommentSaved = onCommentSaved;
    }

    @FXML
    public void initialize() {
        saveButton.setOnAction(event -> handleSaveComment());
        cancelButton.setOnAction(event -> closeWindow());
    }
    private void handleSaveComment() {
        String content = commentField.getText();
        if (content.isEmpty()) {
            showAlert("Ошибка", "Пустой комментарий", "Пожалуйста, введите текст комментария.");
            return;
        }

        int userId = getUserId();
        if (userId == -1) {
            showAlert("Ошибка", "Не удалось определить пользователя", "Пожалуйста, войдите в систему.");
            return;
        }

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword())) {
            String query = "INSERT INTO comments (news_id, user_id, content) VALUES ((SELECT id FROM news WHERE title = ?), ?, ?)";
            try (PreparedStatement stmt = conn.prepareStatement(query)) {
                stmt.setString(1, newsTitle);
                stmt.setInt(2, userId);
                stmt.setString(3, content);
                stmt.executeUpdate();
            }

            if (onCommentSaved != null) onCommentSaved.run();
            closeWindow();
        } catch (Exception e) {
            e.printStackTrace();
            showAlert("Ошибка", "Не удалось сохранить комментарий", "Пожалуйста, попробуйте снова.");
        }
    }

    private void closeWindow() {
        Stage stage = (Stage) saveButton.getScene().getWindow();
        stage.close();
    }

    private void showAlert(String title, String header, String content) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(header);
        alert.setContentText(content);
        alert.showAndWait();
    }
    private int getUserId() {
        if (Login.isGuest()) {
            return -1;
        }

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement stmt = conn.prepareStatement("SELECT id FROM users WHERE username = ?")) {

            stmt.setString(1, Login.getCurrentUsername());
            try (ResultSet rs = stmt.executeQuery()) {
                if (rs.next()) {
                    return rs.getInt("id");
                } else {
                    return -1;
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            return -1;
        }
    }
}
