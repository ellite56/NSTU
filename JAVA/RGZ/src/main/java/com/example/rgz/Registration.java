package com.example.rgz;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Alert;
import javafx.scene.control.Button;
import javafx.scene.control.PasswordField;
import javafx.scene.control.TextField;
import javafx.scene.text.Text;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.*;

public class Registration {

    @FXML
    private TextField usernameField;
    @FXML
    private PasswordField passwordField;
    @FXML
    private PasswordField confirmPasswordField;
    @FXML
    private Button registerButton;
    @FXML
    private Text messageText;

    @FXML
    public void initialize() {
        registerButton.setOnAction(event -> handleRegister());
    }

    private void handleRegister() {
        String username = usernameField.getText();
        String password = passwordField.getText();
        String confirmPassword = confirmPasswordField.getText();

        if (password.equals(confirmPassword) && !passwordField.getText().isEmpty() && !confirmPasswordField.getText().isEmpty()) {
            if (registerUser(username, password)) {
                try {
                    Stage stage = (Stage) registerButton.getScene().getWindow();//переход на экран входа
                    FXMLLoader loader = new FXMLLoader(getClass().getResource("login.fxml"));
                    stage.setScene(new Scene(loader.load()));
                    stage.show();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            } else {
                passwordField.clear();
                confirmPasswordField.clear();
            }
        } else {
            passwordField.clear();
            confirmPasswordField.clear();
            showAlert("Ошибка", "Ошибка регистрации", "Пароли не совпадают.");
        }
    }

    private void showAlert(String title, String headerText, String contentText) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle(title);
        alert.setHeaderText(headerText);
        alert.setContentText(contentText);
        alert.showAndWait();
    }

    private boolean userExists(String username) {//проверка
        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement preparedStatement = conn.prepareStatement("SELECT * FROM users WHERE username = ?")) { // Используем 'username' вместо 'login'
            preparedStatement.setString(1, username);
            try (ResultSet resultSet = preparedStatement.executeQuery()) {
                return resultSet.next();
            }
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }

    private boolean registerUser(String username, String password) {
        if (userExists(username)) {
            showAlert("Ошибка", "Ошибка регистрации", "Пользователь с таким именем уже существует.");
            return false;
        }

        try (Connection conn = DriverManager.getConnection(DataBaseInfo.getUrl(), DataBaseInfo.getUser(), DataBaseInfo.getPassword());
             PreparedStatement preparedStatement = conn.prepareStatement(
                     "INSERT INTO users(username, pass, is_admin) VALUES (?, ?, ?)")) {
            preparedStatement.setString(1, username);
            preparedStatement.setString(2, password);
            preparedStatement.setBoolean(3, false); // Пользователь не является администратором
            preparedStatement.executeUpdate();
            return true;
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
}
