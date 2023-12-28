#pragma one
#ifndef COMMON_H
#define COMMON_H

// Khối mã nguồn của header file

#include <iostream>
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
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <fstream>
/*0 -- easy, 1-- normal, 2-- difficult, 3-- veryhard*/
struct Question {
    int id;
    char content[100];
    char choices1[100];
    char choices2[100];
    char choices3[100];
    char choices4[100];
    char correctAnswer[100];
    char topic[100];
    int level;
};

struct User {
    int id;
    char username[100];
    char password[100];
};

/*  status: 0 = ok, 1 = tessing, 3 = end*/
struct Room
{
    int id;
    char name[100];
    int status ;
    int timeDuration;
    int numberQuestion;
    char user[100];
    int score;
    int level;
    char topic[100];
};

#define DB_DIR "user.db"
#define MAX_SIZE 1000
#define DB_FILE_NAME "user.db"


#define PORT      8001

#define LOGIN "login"
#define REGISTER "register"
#define CREATE_ROOM "createroom"
#define UPDATE_DURATION "update_duration"
#define UPDATE_TIME_END_ROOM "update_time_end_room"
#define JOIN_ROOM "join_rom"
#define VIEW_STATUS_ROOM "view_status_room"
#define VIEW_RUSULT_ROOM "view_result_room"
#define START_EXAM "start_exam"
#define SUBMIT_EXAM "submit_exam"
#define TRAINING_MODE "training_mode"
#define RETURN_MAIN_MENU "return_main" 
#define LOGOUT "logout"
#define NUMBER_QUESTION_TRAINING_MODE 20
#define TIME_FOR_EACH_QUESTION  5 


#endif // MY_HEADER_FILE_H
