module com.example.rgz {
    requires javafx.controls;
    requires javafx.fxml;
    requires java.sql;

    opens com.example.rgz to javafx.fxml;
    exports com.example.rgz;
}