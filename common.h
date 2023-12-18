#pragma one

struct Question {
    std::string content;
    std::string choices[4];
    int correctAnswer;
};

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
#define NUMBER_QUESTION_TRAINING_MODE 20
#define TIME_FOR_EACH_QUESTION  5 


