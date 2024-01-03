// Server.h
#pragma once
#include "common.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <vector>

class Server {
public:
    Server();
    void start();
    void handleCreateExamRoom(int clientSocket);
    void handleSetNumberOfQuestions(int clientSocket);
    void handleSetExamDuration(int clientSocket);
    void handleLogin(int clientSocket);
    void handleRegistration(int clientSocket);
    void handleViewRusultRoom(int clientSocket);
    void handleTrainningMode(int clientSocket);
    void handleJoinRoom(int clientSocket);
    void handleStartExam(int clientSocket,  int id_room, std::string user);
    void handleViewStatusRoom(int clientSocket);
    void handleLogout(int clientSocket);
    void closeConnected()
    {
        isConnected = false;
    }
    ~Server();

private:
    bool isAdmin;
    bool isConnected;
    int serverSocket;
    sockaddr_in serverAddress;

    // User data structure
    struct UserData {
        std::string username;
        std::string password;
    };

    // Vector to store user data
    std::vector<UserData> users;

    // Helper functions
    bool loginUser(const std::string& username, const std::string& password);
    bool registerUser(const std::string& username, const std::string& password);
};
