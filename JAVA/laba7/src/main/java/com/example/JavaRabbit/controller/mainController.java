package com.example.JavaRabbit.controller;

import com.example.JavaRabbit.Model.Rabbit;
import com.example.JavaRabbit.Model.AlbinoRabbit;
import javafx.application.Platform;
import javafx.beans.binding.Bindings;
import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.Node;
import javafx.scene.control.*;
import javafx.scene.input.KeyCode;
import javafx.scene.input.KeyEvent;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.Pane;
import javafx.scene.layout.VBox;
import javafx.stage.FileChooser;
import java.io.*;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;
import java.util.logging.Level;
import java.util.logging.Logger;

public class mainController {
    @FXML
    public BorderPane mainPane;
    @FXML private Pane rabbitsPane;
    @FXML private Label timeLabel;
    @FXML private Label statisticLabel;
    @FXML private RadioButton showTimeButton;
    @FXML private RadioButton hideTimeButton;
    @FXML private Button startButton;
    @FXML private Button stopButton;
    @FXML private BorderPane controlPanel;
    @FXML private CheckBox resultWindowCheckBox;
    @FXML private Button OkButton;
    @FXML private TextField N1TextField;
    @FXML private Slider P1Slider;
    @FXML private Slider P2PercentageSlider;
    @FXML private Label P1ValueLabel;
    @FXML private Label P2ValueLabel;
    @FXML private TextField commonLifetimeTextField;
    @FXML private TextField albinoLifetimeTextField;
    @FXML private Button showObjectsButton;
    @FXML private Button stopAlbinoAIButton;
    @FXML private Button resumeAlbinoAIButton;
    @FXML private Button stopCommonAIButton;
    @FXML private Button resumeCommonAIButton;
    @FXML public ComboBox<String> commonPriorityDropdown;
    @FXML public ComboBox<String> albinoPriorityDropdown;
    @FXML private MenuItem connectedListMenuItem;

    private Socket clientSocket;
    private ObjectOutputStream outputStream;
    private String clientID;
    private List<String> connectedClients = new ArrayList<>();
    private Habitat habitat;
    private static final int PORT_RANGE_START = 5000;
    private static final int PORT_RANGE_END = 6000;

    // Переменные для хранения параметров подключения
    private String serverIP = "127.0.0.1";
    private int serverPort = 5000;

    @FXML
    public void initialize() {
        habitat = Habitat.getInstance();
        habitat.setController(this);
        showTimeButton.setSelected(true);
        startButton.setDisable(true);
        stopButton.setDisable(true);
        mainPane.addEventFilter(KeyEvent.KEY_PRESSED, this::keyPressed);
        P1Slider.setValue(10);
        P2PercentageSlider.setValue(10);
        N1TextField.setText("1");
        P1ValueLabel.textProperty().bind(Bindings.format("%.0f", P1Slider.valueProperty()));
        P2ValueLabel.textProperty().bind(Bindings.format("%.0f", P2PercentageSlider.valueProperty()));
        commonPriorityDropdown.setValue("1");
        albinoPriorityDropdown.setValue("1");
        connectedListMenuItem.setOnAction(event -> showConnectedClients());

        // Показываем диалог выбора сервера перед подключением
        if (!showServerConnectionDialog()) {
            System.exit(0); // Завершаем приложение, если пользователь отменил диалог
        }

        new Thread(this::listenForUpdates).start();
    }

    private boolean showServerConnectionDialog() {
        Dialog<ButtonType> dialog = new Dialog<>();
        dialog.setTitle("Подключение к серверу");
        dialog.setHeaderText("Введите IP-адрес и порт сервера:");

        //создание содер диалога
        VBox content = new VBox(10);
        Label ipLabel = new Label("IP-адрес:");
        TextField ipField = new TextField(serverIP);
        Label portLabel = new Label("Порт:");
        TextField portField = new TextField(String.valueOf(serverPort));
        content.getChildren().addAll(ipLabel, ipField, portLabel, portField);//добавляем в контейнер

        dialog.getDialogPane().setContent(content);
        dialog.getDialogPane().getButtonTypes().addAll(ButtonType.OK, ButtonType.CANCEL);

        Optional<ButtonType> result = dialog.showAndWait(); // Теперь возвращает Optional<ButtonType>

        if (result.isPresent() && result.get() == ButtonType.OK) {
            serverIP = ipField.getText();
            try {
                serverPort = Integer.parseInt(portField.getText());
                if (serverPort < 1024 || serverPort > 65535) {
                    showAlert("Ошибка", "Порт должен быть в диапазоне 1024–65535.");
                    return false;
                }
            } catch (NumberFormatException e) {
                showAlert("Ошибка", "Порт должен быть числом.");
                return false;
            }
            return true;
        } else {
            return false;
        }
    }

    private String showServerIPDialog() {
        TextInputDialog dialog = new TextInputDialog("127.0.0.1");
        dialog.setTitle("Выбор сервера");
        dialog.setHeaderText("Введите IP-адрес сервера:");
        dialog.setContentText("IP:");

        Optional<String> result = dialog.showAndWait();//отобрадение
        return result.orElse(null);
    }

    private void listenForUpdates() {
        //подключение к северу
        int port = PORT_RANGE_START;
        while (clientSocket == null && port <= PORT_RANGE_END) {
            try {
                clientSocket = new Socket(serverIP, serverPort);//создаем сокет
                break;
            } catch (IOException e) {
                port++;
                if (port > PORT_RANGE_END) {
                    showAlert("Ошибка", "Не удалось подключиться к серверу.");
                    return;
                }
            }
        }

        try {
            ObjectInputStream inputStream = new ObjectInputStream(clientSocket.getInputStream());//читаем данные с сервера
            Object firstObject = inputStream.readObject();
            if (firstObject instanceof String) {
                setClientID((String) firstObject);
            }

            while (true) {
                try {
                    Object receivedData = inputStream.readObject();
                    if (receivedData instanceof SettingsDto) {//настройки
                        Platform.runLater(() -> receiveAndApplySettings((SettingsDto) receivedData));
                    } else if (receivedData instanceof TargetedSettings) {
                        TargetedSettings targetedSettings = (TargetedSettings) receivedData;
                        if (targetedSettings.getClientKey().equals(clientID)) {
                            Platform.runLater(() -> receiveAndApplySettings(targetedSettings.getSettings()));
                        }
                    } else if (receivedData instanceof List<?>) {//список клиентов
                        List<?> list = (List<?>) receivedData;
                        if (!list.isEmpty() && list.get(0) instanceof String) {
                            Platform.runLater(() -> handleConnectedClientsIDs((List<String>) list));
                        }
                    }
                } catch (EOFException | ClassNotFoundException e) {
                    System.err.println("Ошибка получения данных: " + e.getMessage());
                }
            }
        } catch (IOException | ClassNotFoundException e) {
            e.printStackTrace();
            showAlert("Ошибка", "Не удалось получить данные от сервера.");
        }
    }


    public void setClientID(String id) {
        this.clientID = id;
    }

    private void sendSettingsToServer(String targetClientID) {//получение настроек от клиента
        try {
            if (outputStream == null) {
                outputStream = new ObjectOutputStream(clientSocket.getOutputStream());
            }
            int p1 = Math.round((float) P1Slider.getValue());
            int n1 = Integer.parseInt(N1TextField.getText());
            int p2Percentage = Math.round((float) P2PercentageSlider.getValue());
            String commonPriority = commonPriorityDropdown.getValue();
            String albinoPriority = albinoPriorityDropdown.getValue();
            long commonLifetime = Long.parseLong(commonLifetimeTextField.getText()) * 1000;
            long albinoLifetime = Long.parseLong(albinoLifetimeTextField.getText()) * 1000;

            //создание DTO об
            SettingsDto settingsDto = new SettingsDto(p1, n1, p2Percentage, commonPriority, albinoPriority, commonLifetime, albinoLifetime);
            TargetedSettings targetedSettings = new TargetedSettings(targetClientID, settingsDto);
            outputStream.writeObject(targetedSettings);
            outputStream.flush();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void receiveAndApplySettings(SettingsDto settings) {
        setP1SliderValue(settings.getP1());
        setN1TextFieldValue(String.valueOf(settings.getN1()));
        setP2PercentageSliderValue(settings.getP2Percentage());
        setCommonPriority(Integer.parseInt(settings.getCommonPriority()));
        setAlbinoPriority(Integer.parseInt(settings.getAlbinoPriority()));
        setCommonLifetimeTextFieldValue(String.valueOf(settings.getCommonLifetime() / 1000));
        setAlbinoLifetimeTextFieldValue(String.valueOf(settings.getAlbinoLifetime() / 1000));
    }

    private void showConnectedClients() {
        if (connectedClients.isEmpty()) {
            showAlert("Нет клиентов", "Нет подключенных клиентов для отправки настроек.");
            return;
        }
        ChoiceDialog<String> dialog = new ChoiceDialog<>(null, connectedClients);
        dialog.setTitle("Подключенные клиенты");
        dialog.setHeaderText("Выберите клиент:");
        dialog.setContentText("ID клиента:");
        Optional<String> result = dialog.showAndWait();
        result.ifPresent(this::sendSettingsToServer);
    }

    private void handleConnectedClientsIDs(List<String> clientIDs) {//обновление списка подкл клиентов
        connectedClients.clear();
        connectedClients.addAll(clientIDs);
    }

    private void showAlert(String title, String message) {//модальное окно об ошибке
        Platform.runLater(() -> {
            Alert alert = new Alert(Alert.AlertType.ERROR);
            alert.setTitle(title);
            alert.setHeaderText(null);
            alert.setContentText(message);
            alert.showAndWait();
        });
    }

    public Button getStopButton() { return stopButton; }
    public Button getStartButton() { return startButton; }

    @FXML
    public void handlePriorityChange() {
        String commonPriority = commonPriorityDropdown.getValue();
        String albinoPriority = albinoPriorityDropdown.getValue();
        Habitat.setCommonPriority(Integer.parseInt(commonPriority));
        Habitat.setAlbinoPriority(Integer.parseInt(albinoPriority));
    }

    @FXML
    public void stopAlbinoAI(ActionEvent event) {
        if (Habitat.startFlag) {
            synchronized (habitat) {
                habitat.stopAlbinoAI();
            }
            stopAlbinoAIButton.setDisable(true);
            resumeAlbinoAIButton.setDisable(false);
        }
    }

    @FXML
    public void resumeAlbinoAI(ActionEvent event) {
        if (Habitat.startFlag) {
            synchronized (habitat) {
                habitat.resumeAlbinoAI();
                habitat.notifyAll();
                stopAlbinoAIButton.setDisable(false);
                resumeAlbinoAIButton.setDisable(true);
            }
        }
    }

    @FXML
    public void stopCommonAI(ActionEvent event) {
        if (Habitat.startFlag) {
            synchronized (habitat) {
                habitat.stopCommonAI();
                stopCommonAIButton.setDisable(true);
                resumeCommonAIButton.setDisable(false);
            }
        }
    }

    @FXML
    public void resumeCommonAI(ActionEvent event) {
        if (Habitat.startFlag) {
            synchronized (habitat) {
                habitat.resumeCommonAI();
                habitat.notifyAll();
                stopCommonAIButton.setDisable(false);
                resumeCommonAIButton.setDisable(true);
            }
        }
    }

    @FXML
    public void startAction() {
        int P1 = (int) P1Slider.getValue();
        if (!Habitat.startFlag && P1 != 0 ) {
            habitat.startAction();
            if (showTimeButton.isSelected()) {
                Habitat.getInstance().showTimeLabel();
            }
            startButton.setDisable(true);
            stopButton.setDisable(false);
        }
    }

    @FXML
    public void stopAction() {
        if (Habitat.startFlag) {
            habitat.stopAction();
            startButton.setDisable(false);
            stopButton.setDisable(true);
        }
    }

    @FXML
    void showTimeButtonClick(ActionEvent event) {
        if (Habitat.startFlag) {
            hideTimeButton.setSelected(false);
            habitat.showTimeLabel();
            habitat.timeFlag = true;
        }
    }

    @FXML
    void hideTimeButtonClick(ActionEvent event) {
        showTimeButton.setSelected(false);
        habitat.hideTimeLabel();
        habitat.timeFlag = false;
    }

    @FXML
    public void keyPressed(KeyEvent event) {
        try {
            if (event.getCode().equals(KeyCode.T)) {
                if (Habitat.startFlag) {
                    toggleTimeLabel();
                }
            } else if (event.getCode().equals(KeyCode.B)) {
                if (!Habitat.startFlag) {
                    habitat.startAction();
                }
            } else if (event.getCode().equals(KeyCode.E)) {
                if (Habitat.startFlag) {
                    habitat.stopAction();
                }
            }
        } catch (Exception e) {
            Logger.getLogger(mainController.class.getName()).log(Level.SEVERE, "Error handling key event", e);
        }
    }

    @FXML
    void resultWindowCheckBoxClick(ActionEvent event) {
        Habitat.setResultWindowFlag(resultWindowCheckBox.isSelected());
    }

    private void toggleTimeLabel() {
        if (showTimeButton.isSelected()) {
            hideTimeButton.setSelected(true);
            hideTimeButtonClick(null);
        } else {
            showTimeButton.setSelected(true);
            showTimeButtonClick(null);
        }
        habitat.timeFlag = !habitat.timeFlag;
    }

    @FXML
    void OkButtonClick(ActionEvent event) {
        if (!validateInputs()) {
        } else {
            startButton.setDisable(false);
            stopButton.setDisable(false);
        }
    }

    private boolean validateInputs() {
        int selectedP1 = (int) P1Slider.getValue();
        int selectedN1;
        try {
            selectedN1 = Integer.parseInt(N1TextField.getText());
            if (selectedN1 <= 0 || selectedN1 > 100) {
                Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
                alert.setTitle("Confirm Input");
                alert.setHeaderText("Invalid Input");
                alert.setContentText("N1 must be greater than 0 and less than or equal to 100.\nDo you want to continue with default entry?");
                Optional<ButtonType> result = alert.showAndWait();
                if (result.isPresent() && result.get() == ButtonType.OK) {
                    selectedN1 = 1;
                } else {
                    return false;
                }
            }
        } catch (NumberFormatException e) {
            showInvalidInputAlert("N1 must be a valid integer.");
            return false;
        }
        Habitat.setP1(selectedP1);
        Habitat.setN1(selectedN1);
        Habitat.getInstance().setP2Percentage(Math.round((float) P2PercentageSlider.getValue()));
        if (!validateLifetimeInput(commonLifetimeTextField.getText(), "Common Rabbit")) return false;
        if (!validateLifetimeInput(albinoLifetimeTextField.getText(), "Albino Rabbit")) return false;
        return true;
    }

    private boolean validateLifetimeInput(String input, String type) {
        long commonLifetime, albinoLifetime;
        try {
            commonLifetime = Long.parseLong(commonLifetimeTextField.getText()) * 1000;
            albinoLifetime = Long.parseLong(albinoLifetimeTextField.getText()) * 1000;
            if (commonLifetime <= 0 || albinoLifetime <= 0) {
                showInvalidInputAlert("Lifetime values must be greater than 0.");
                return false;
            }
        } catch (NumberFormatException e) {
            showInvalidInputAlert("Invalid input. Please enter a valid number.");
            return false;
        }
        habitat.setLifetimeValues(commonLifetime, albinoLifetime);
        return true;
    }

    private void showInvalidInputAlert(String message) {
        Alert alert = new Alert(Alert.AlertType.ERROR);
        alert.setTitle("Invalid Input");
        alert.setHeaderText(null);
        alert.setContentText(message);
        alert.showAndWait();
    }

    public Pane getRabbitsPane() {
        return rabbitsPane;
    }

    public Label getTimeLabel() {
        return timeLabel;
    }

    public Label getStatistic() {
        return statisticLabel;
    }

    public void printLabel(String text) {
        timeLabel.setText(text);
    }

    @FXML
    void showCurrentObjects(ActionEvent event) {
        Habitat.getInstance().showCurrentObjects();
    }

    @FXML
    void saveObjects(ActionEvent event) {
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Save Objects");
        File file = fileChooser.showSaveDialog(null);
        if (file != null) {
            Habitat.getInstance().saveObjectsToFile(file);
        }
    }

    @FXML
    void loadObjects(ActionEvent event) {
        Habitat.getInstance().stopAction();
        FileChooser fileChooser = new FileChooser();
        fileChooser.setTitle("Load Objects");
        File file = fileChooser.showOpenDialog(null);
        if (file != null) {
            long currentTime = System.currentTimeMillis();
            Habitat.getInstance().loadObjectsFromFile(file, currentTime);
        }
    }

    @FXML
    void openConsole(ActionEvent event) {
        Dialog<String> dialog = new Dialog<>();
        dialog.setTitle("Console");
        dialog.getDialogPane().getButtonTypes().addAll(ButtonType.CLOSE);
        Node closeButton = dialog.getDialogPane().lookupButton(ButtonType.CLOSE);
        closeButton.setVisible(false);
        Label introductionLabel = new Label();
        introductionLabel.setPrefWidth(400);
        introductionLabel.setStyle("-fx-background-color: black; -fx-text-fill: white;");
        introductionLabel.setText("Welcome to the Console!\nThis is a sample console application.\nYou can enter commands below and press Enter to execute them.\nC:\\CodeXProject\\Reduce albino by %");
        TextArea textArea = new TextArea();
        textArea.setPrefWidth(400);
        textArea.setPrefHeight(300);
        textArea.setStyle("-fx-control-inner-background: black; -fx-text-fill: white;");
        textArea.setEditable(true);
        VBox content = new VBox(introductionLabel, textArea);
        dialog.getDialogPane().setContent(content);
        Platform.runLater(textArea::requestFocus);
        dialog.getDialogPane().addEventFilter(KeyEvent.KEY_PRESSED, e -> {
            if (e.getCode() == KeyCode.ENTER && !e.isShiftDown()) {
                e.consume();
                try (BufferedReader reader = new BufferedReader(new StringReader(textArea.getText()))) {
                    String[] lines = reader.lines().toArray(String[]::new);
                    String userInput = lines[lines.length - 1];
                    String response = processCommand(userInput);
                    textArea.appendText("\n" + response + "\n");
                } catch (IOException ex) {
                    ex.printStackTrace();
                }
            }
        });
        dialog.showAndWait();
    }

    public String processCommand(String command) {
        if (command.toLowerCase().startsWith("reduce albino by")) {
            try {
                int percentage = Integer.parseInt(command.replaceAll("[^0-9]", ""));
                double percent = percentage / 100.0;
                reduceAlbinoRabbits(percent);
                return "Reduced " + percentage + "% of albino rabbits.";
            } catch (NumberFormatException e) {
                return "Invalid percentage format.";
            }
        } else {
            return "Unknown command.";
        }
    }

    private void reduceAlbinoRabbits(double percent) {
        List<Rabbit> albinoRabbitsToRemove = new ArrayList<>();
        for (Rabbit rabbit : Habitat.getInstance().getListRabbits()) {
            if (rabbit instanceof AlbinoRabbit) {
                albinoRabbitsToRemove.add(rabbit);
            }
        }
        int totalAlbinoRabbits = albinoRabbitsToRemove.size();
        int numToRemove = (int) (totalAlbinoRabbits * percent);
        for (int i = 0; i < numToRemove; i++) {
            if (!albinoRabbitsToRemove.isEmpty()) {
                Rabbit rabbitToRemove = albinoRabbitsToRemove.remove(0);
                getRabbitsPane().getChildren().remove(rabbitToRemove.getImageView());
                Habitat.getInstance().getListRabbits().remove(rabbitToRemove);
            }
        }
    }

    public void setP1SliderValue(double value) {
        P1Slider.setValue(value);
    }

    public void setN1TextFieldValue(String value) {
        N1TextField.setText(value);
    }

    public void setP2PercentageSliderValue(double value) {
        P2PercentageSlider.setValue(value);
    }

    public void setCommonLifetimeTextFieldValue(String value) {
        commonLifetimeTextField.setText(value);
    }

    public void setAlbinoLifetimeTextFieldValue(String value) {
        albinoLifetimeTextField.setText(value);
    }

    public void setCommonPriority(int priority) {
        commonPriorityDropdown.setValue(String.valueOf(priority));
    }

    public void setAlbinoPriority(int priority) {
        albinoPriorityDropdown.setValue(String.valueOf(priority));
    }

    @FXML
    private void handleSaveToDatabase() {
        Habitat.getInstance().saveRabbitsToDatabase();
    }

    @FXML
    private void handleLoadFromDatabase() {
        Alert alert = new Alert(Alert.AlertType.CONFIRMATION);
        alert.setTitle("Load from Database");
        alert.setHeaderText("Choose Rabbit Type");
        alert.setContentText("Do you want to retrieve Albino or Common rabbits?");

        ButtonType albinoButton = new ButtonType("Albino");
        ButtonType commonButton = new ButtonType("Common");
        ButtonType cancelButton = new ButtonType("Cancel", ButtonBar.ButtonData.CANCEL_CLOSE);

        alert.getButtonTypes().setAll(albinoButton, commonButton, cancelButton);

        Optional<ButtonType> result = alert.showAndWait();
        if (result.isPresent()) {
            if (result.get() == albinoButton) {
                Habitat.getInstance().loadRabbitsFromDatabase("Albino");
            } else if (result.get() == commonButton) {
                Habitat.getInstance().loadRabbitsFromDatabase("Common");
            }
        }
    }

}

