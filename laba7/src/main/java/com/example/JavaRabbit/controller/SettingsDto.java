package com.example.JavaRabbit.controller;
import java.io.Serializable;

public class SettingsDto implements Serializable {
    private int p1;
    private int n1;
    private int p2Percentage;
    private String commonPriority;
    private String albinoPriority;
    private long commonLifetime;
    private long albinoLifetime;

    // Конструктор
    public SettingsDto(int p1, int n1, int p2Percentage,
                       String commonPriority, String albinoPriority,
                       long commonLifetime, long albinoLifetime) {
        this.p1 = p1;
        this.n1 = n1;
        this.p2Percentage = p2Percentage;
        this.commonPriority = commonPriority;
        this.albinoPriority = albinoPriority;
        this.commonLifetime = commonLifetime;
        this.albinoLifetime = albinoLifetime;
    }

    // Геттеры
    public int getP1() { return p1; }
    public int getN1() { return n1; }
    public int getP2Percentage() { return p2Percentage; }
    public String getCommonPriority() { return commonPriority; }
    public String getAlbinoPriority() { return albinoPriority; }
    public long getCommonLifetime() { return commonLifetime; }
    public long getAlbinoLifetime() { return albinoLifetime; }
}