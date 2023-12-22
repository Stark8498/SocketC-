#pragma once

#include <iostream>
#include "common.h"
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <sstream>
#include <chrono>
#include <thread>
#include <chrono>
#include <thread>
#include <atomic>



class Client {
public:
    Client();
    void connectToServer();
    void showMenu();
    void showSubMenu();
    void disconnect();
    void createExamRoom();
    void updateEndTime();
    void joinRoom();
    void viewStatusRoom();
    void resultRoom();
    void startExam();
    int askQuestion(Question q);
    void trainingMode();
    std::vector<Question> getQuestion();
    void setNumberOfQuestions();
    void setExamDuration();
    void showTest(std::vector<Question> question, int timeDuration);
    void handleMenuChoice(int choice);
    ~Client();

private:
    int clientSocket;
    sockaddr_in serverAddress;
    bool isConnected;

    void login();
    void registerUser();
};
