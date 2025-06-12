package com.example.rgz;

public class DataBaseInfo {
    private static final String URL = "jdbc:postgresql://localhost:5432/News";
    private static final String USER = "postgres";
    private static final String PASSWORD = "41953";

    public static String getUrl() {
        return URL;
    }

    public static String getUser() {
        return USER;
    }

    public static String getPassword() {
        return PASSWORD;
    }
}
