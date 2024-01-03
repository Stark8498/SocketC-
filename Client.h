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
#include <vector>
#include <regex.h>
#include <regex>
#include "DbSqlite.h"

class Client
{
public:
    Client();
    void Logout();
    void connectToServer();
    void showMenu();
    void showSubMenu();
    void disconnect();
    void createExamRoom();
    void joinRoom();
    void viewStatusRoom();
    void resultRoom();
    void startExam(std::string joinRoom, int numberQuestion, int timeDuration, std::string topic,
                   int easy, int normal, int difficult, int veryhard);
    std::string askQuestion(Question q);
    void trainingMode();
    std::vector<Question> getQuestion();
    void setNumberOfQuestions();
    void setExamDuration();
    void viewLogTrace();
    void showTest(std::vector<Question> question, int timeDuration, int numberQuestion,
                    int &correct_answer,
                    bool isTraining);
    void handleMenuChoice(int choice);
    ~Client();

private:
    bool isAdmin;
    bool isLogin;
    std::ofstream outfile;
    int clientSocket;
    sockaddr_in serverAddress;
    bool isConnected;

    void login();
    void registerUser();
};
