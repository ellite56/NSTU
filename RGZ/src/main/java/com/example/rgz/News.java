package com.example.rgz;

import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;

import java.text.SimpleDateFormat;
import java.util.Date;

public class News {
    private int id;
    private final StringProperty title;
    private final StringProperty content;
    private final StringProperty createdAt;
    private final StringProperty updatedAt;

    public News(String title, String content, String createdAt, String updatedAt) {
        this.title = new SimpleStringProperty(title);
        this.content = new SimpleStringProperty(content);
        this.createdAt = new SimpleStringProperty(formatDate(createdAt));
        this.updatedAt = new SimpleStringProperty(formatDate(updatedAt));
    }

    private String formatDate(String dateTime) {
        try {
            SimpleDateFormat inputFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSSSSS");
            Date date = inputFormat.parse(dateTime);
            SimpleDateFormat outputFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            return outputFormat.format(date);
        } catch (Exception e) {
            return dateTime;
        }
    }

    public int getId() {
        return id;
    }

    public String getTitle() {
        return title.get();
    }

    public StringProperty titleProperty() {
        return title;
    }

    public String getContent() {
        return content.get();
    }

    public StringProperty contentProperty() {
        return content;
    }

    public String getCreatedAt() {
        return createdAt.get();
    }

    public StringProperty createdAtProperty() {
        return createdAt;
    }

    public String getUpdatedAt() {
        return updatedAt.get();
    }

    public StringProperty updatedAtProperty() {
        return updatedAt;
    }

    public void setId(int id) {
        this.id = id;
    }

    public void setTitle(String title) {
        this.title.set(title);
    }

    public void setContent(String content) {
        this.content.set(content);
    }
}
