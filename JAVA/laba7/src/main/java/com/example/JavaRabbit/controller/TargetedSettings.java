package com.example.JavaRabbit.controller;

import java.io.Serializable;

public class TargetedSettings implements Serializable {
    private static final long serialVersionUID = 1L;
    private final String clientKey;//идентификатор клиента
    private final SettingsDto settings;

    public TargetedSettings(String clientKey, SettingsDto settings) {
        this.clientKey = clientKey;
        this.settings = settings;
    }

    public String getClientKey() {
        return clientKey;
    }

    public SettingsDto getSettings() {
        return settings;
    }
}