package com.example.JavaRabbit.controller;

import java.io.IOException;
import java.io.ObjectOutputStream;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class TCPServer {
    private static final int PORT_RANGE_START = 5000;
    private static final int PORT_RANGE_END = 8000;
    private static final Map<String, ObjectOutputStream> clientsMap = new HashMap<>();
    private static int clientIdCounter = 1;

    public static void main(String[] args) {
        int port = PORT_RANGE_START;
        ServerSocket serverSocket = null;

        while (serverSocket == null && port <= PORT_RANGE_END) {
            try {
                serverSocket = new ServerSocket(port++);
            } catch (IOException ignored) {}
        }

        if (serverSocket == null) {
            System.err.println("Не удалось найти свободный порт в указанном диапазоне.");
            return;
        }

        System.out.println("Сервер запущен. Порт: " + serverSocket.getLocalPort());

        while (true) {
            try {
                Socket clientSocket = serverSocket.accept();
                //получене инфы о клиенте
                InetAddress clientAddress = clientSocket.getInetAddress();
                String ipAddress = clientAddress.getHostAddress();
                int clientPort = clientSocket.getPort();
                //это как раз чтобы понимать где какой клиент
                String clientID = "Client-" + (clientIdCounter++) + " (" + ipAddress + ":" + clientPort + ")";

                //отправка id по клиенту
                ObjectOutputStream oos = new ObjectOutputStream(clientSocket.getOutputStream());
                oos.writeObject(clientID);
                oos.flush();
                clientsMap.put(clientID, oos);

                new Thread(() -> handleClient(clientSocket, clientID)).start();
                sendConnectedClientsList();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void handleClient(Socket clientSocket, String clientID) {
        try {
            var inputStream = new java.io.ObjectInputStream(clientSocket.getInputStream());

            while (true) {
                Object receivedData = inputStream.readObject();//считываем объект

                if (receivedData instanceof TargetedSettings targetedSettings) {//отправление определенному клиенту
                    //TargetedSettings targetedSettings = (TargetedSettings) receivedData;
                    String targetID = targetedSettings.getClientKey();

                    ObjectOutputStream targetStream = clientsMap.get(targetID);
                    if (targetStream != null) {
                        targetStream.writeObject(targetedSettings);
                        targetStream.flush();
                    }
                } else if (receivedData instanceof SettingsDto) {//отправление всем клиентам
                    transmitSettingsToClients((SettingsDto) receivedData);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            try {//очистка
                clientSocket.close();
                clientsMap.remove(clientID);
                sendConnectedClientsList();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void transmitSettingsToClients(SettingsDto settings) {
        for (var oos : clientsMap.values()) {
            try {
                oos.writeObject(settings);
                oos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private static void sendConnectedClientsList() {//обновление списка подкл клиентов
        List<String> clientIDs = new ArrayList<>(clientsMap.keySet());
        for (var oos : clientsMap.values()) {
            try {
                oos.writeObject(clientIDs);
                oos.flush();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}