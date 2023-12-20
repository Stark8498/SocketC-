#include "common.h"
#define CREATE_USER_TABLE_SQL \
    "CREATE TABLE USERS (" \
    "id_user INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "username VARCHAR(255) NOT NULL, " \
    "password VARCHAR(255) NOT NULL,"\
    "points VARCHAR(255) NOT NULL); " 

#define CREATE_USER_QUESTION_SQL \
    "CREATE TABLE QUESTIONS (" \
    "id_question INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "question VARCHAR(255) NOT NULL, " \
    "answer_a VARCHAR(255) NOT NULL, " \
    "answer_b VARCHAR(255) NOT NULL, " \
    "answer_c VARCHAR(255) NOT NULL, " \
    "answer_d VARCHAR(255) NOT NULL, " \
    "correct_answer VARCHAR(255) NOT NULL);"
#define CREATE_ROOM_TABLE_SQL \
    "CREATE TABLE ROOMS (" \
    "id_room INTEGER PRIMARY KEY AUTOINCREMENT, " \
    "nameroom VARCHAR(255) NOT NULL, " \
    "status INTEGER,"\
    "timeduration INTEGER),"\
    "score INTEGER),"\
    "user VARCHAR(255) NOT NULL),"\
    "numberofquestion INTEGER  );" 


#define INSERT_USER "INSERT INTO User \
        (username, password) VALUES ('%s', '%s');"

#define INSERT_QUESTION "INSERT INTO User \
        (question, points, answer_a, answer_b, answer_c, answer_d) \
        VALUES ('%s', '%s',%s', '%s',%s', '%s');"

#define SQL_SELECT_ID_USER "SELECT id_user FROM USERS WHERE username = '%s' AND password = '%s';"
#define SELECT_TABLE_QUESTION "SELECT * FROM QUESTIONS ;"
#define SELECT_TABLE_QUESTION "SELECT * FROM ROOMS ;"
#define SELECT_ROOM_ISABLE "SELECT nameroom FROM ROOM WHERE status  "





