package com.example.rgz;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import java.io.IOException;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

public class Login {

    @FXML
    private TextField loginField;
    @FXML
    private PasswordField passwordField;
    @FXML
    private Button loginButton;
    @FXML
    private Button guestLoginButton;

    private boolean isAdminLogin = false;

    private static String currentUsername;
    private static boolean isGuest = false;

    public void setAdminLogin(boolean isAdmin) {
        this.isAdminLogin = isAdmin;
    }

    public static String getCurrentUsername() {
        return currentUsername;
    }

    public static boolean isGuest() {
        return isGuest;
    }

    @FXML
    public void handleLogin() {
        String username = loginField.getText();
        String password = passwordField.getText();

        if (authenticateUser(username, password, isAdminLogin)) {
            try {
                currentUsername = username;

                isGuest = false;

                Stage stage = (Stage) loginButton.getScene().getWindow();
                FXMLLoader loader;

                if (isAdminLogin) {
                    loader = new FXMLLoader(getClass().getResource("news_admin.fxml"));
                } else {
                    loader = new FXMLLoader(getClass().getResource("news_users.fxml"));
                }

                stage.setScene(new Scene(loader.load()));
                stage.show();
            } catch (IOException e) {
                e.printStackTrace();
            }
        } else {
            showAlert("Ошибка", "Ошибка входа", "Неверный логин, пароль или права администратора.");
        }
    }

    private boolean authenticateUser(String username, String password, boolean isAdminLogin) {
        String query = "SELECT * FROM users WHERE username = ? AND pass = ? AND is_admin = ?";
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement preparedStatement = conn.prepareStatement(query)) {

            preparedStatement.setString(1, username);
            preparedStatement.setString(2, password);
            preparedStatement.setBoolean(3, isAdminLogin);

            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                return resultSet.next();
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    @FXML
    public void handleGuestLogin() {
        isGuest = true;

        try {
            Stage stage = (Stage) guestLoginButton.getScene().getWindow();
            FXMLLoader loader = new FXMLLoader(getClass().getResource("news_users.fxml"));
            stage.setScene(new Scene(loader.load()));
            stage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private void showAlert(String title, String headerText, String contentText) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(headerText);
        alert.setContentText(contentText);
        alert.showAndWait();
    }
}
