package com.example.rgz;

import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.stage.Stage;
import java.io.IOException;

public class SceneController {

    @FXML
    private Button loginAsAdminButton;
    @FXML
    private Button loginButton;
    @FXML
    private Button registerButton;
    @FXML
    private Button loginAsGuestButton;

    @FXML
    public void initialize() {}

    private static boolean isGuest = false;

    public void init(Stage primaryStage) {
        loginAsAdminButton.setOnAction(event -> handleLoginAsAdmin(primaryStage));
        loginButton.setOnAction(event -> handleLogin(primaryStage));
        registerButton.setOnAction(event -> handleRegister(primaryStage));
        loginAsGuestButton.setOnAction(event -> handleLoginAsGuest(primaryStage));
    }

    private void handleLoginAsAdmin(Stage primaryStage) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("login.fxml"));
            primaryStage.setScene(new Scene(loader.load()));

            Login loginController = loader.getController();
            loginController.setAdminLogin(true);

            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleLogin(Stage primaryStage) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("login.fxml"));
            primaryStage.setScene(new Scene(loader.load()));

            Login loginController = loader.getController();
            loginController.setAdminLogin(false);

            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public void handleRegister(Stage primaryStage) {
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("registration.fxml"));
            primaryStage.setScene(new Scene(loader.load()));
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void handleLoginAsGuest(Stage primaryStage) {
        isGuest = true;
        try {
            FXMLLoader loader = new FXMLLoader(getClass().getResource("news_users.fxml"));
            primaryStage.setScene(new Scene(loader.load()));
            primaryStage.show();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
