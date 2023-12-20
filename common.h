#pragma one

#include <vector>
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include <vector>
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

struct Question {
    int id;
    std::string content;
    std::string choices1;
    std::string choices2;
    std::string choices3;
    std::string choices4;
    int correctAnswer;
};

struct User {
    int id;
    std::string username;
    std::string password;
};

/*  status: 0 = ok, 1 = tessing, 3 = end*/
struct Room
{
    int id;
    std::string name;
    int status ;
    int timeDuration;
    std::string user;
    int score;
    int numberQuestion;
};


#define DB_DIR "user.db"
#define MAX_SIZE 1000
#define DB_FILE_NAME "user.fb"


#define LOGIN "login"
#define REGISTER "register"
#define START_EXAM "start_exame"
#define CREATE_ROOM "createroom"
#define UPDATE_DURATION "update_duration"
#define UPDATE_TIME_END_ROOM "update_time_end_room"
#define JOIN_ROOM "join_rom"
#define VIEW_STATUS_ROOM "view_status_room"
#define VIEW_RUSULT_ROOM "view_result_room"
#define START_EXAM "start_exam"
#define SUBMIT_EXAM "submit_exam"
#define TRAINING_MODE "training_mode"
#define NUMBER_QUESTION_TRAINING_MODE 20
#define TIME_FOR_EACH_QUESTION  5 


