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

class Client
{
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
    void startExam(int numberQuestion, int timeDuration);
    std::string askQuestion(Question q);
    void trainingMode();
    std::vector<Question> getQuestion();
    void setNumberOfQuestions();
    void setExamDuration();
    void viewLogTrace();
    void showTest(std::vector<Question> question, int timeDuration, int numberQuestion);
    void handleMenuChoice(int choice);
    ~Client();

private:
    std::ofstream outfile;
    int clientSocket;
    sockaddr_in serverAddress;
    bool isConnected;

    void login();
    void registerUser();
};
