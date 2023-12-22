#include "common.h"

#define CREATE_USER_TABLE_SQL   "CREATE TABLE USERS (" \
                                    "id_user INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                    "username TEXT, " \
                                    "password TEXT,"\
                                    "points TEXT ); " 

#define CREATE_USER_QUESTION_SQL    "CREATE TABLE QUESTIONS (" \
                                    "id_question INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                    "question TEXT, " \
                                    "answer_a TEXT, " \
                                    "answer_b TEXT, " \
                                    "answer_c TEXT, " \
                                    "answer_d TEXT, " \
                                    "correct_answer TEXT );"
#define CREATE_ROOM_TABLE_SQL    "CREATE TABLE ROOMS (" \
                                    "id_room INTEGER PRIMARY KEY AUTOINCREMENT, " \
                                    "nameroom TEXT, " \
                                    "status BOOL,"\
                                    "timeduration INTEGER,"\
                                    "numberofquestion INTEGER  ,"\
                                    "user TEXT, " \
                                    "score INTEGER );" 


#define INSERT_USER "INSERT INTO User \
        (username, password) VALUES ('%s', '%s');"

#define INSERT_QUESTION "INSERT INTO QUESTIONS \
                         (question, points, answer_a, answer_b, answer_c, answer_d) \
                         VALUES ('%s', '%s', '%s', '%s', '%s', '%s');"


#define INSERT_ROOM "INSERT INTO ROOMS \
        (nameroom, status, timeduration, numberofquestion, user, score) \
        VALUES ('%s', '%d', '%d', '%d', '%s', '%s');"


#define SQL_SELECT_ID_USER "SELECT id_user FROM USERS WHERE username = '%s' AND password = '%s';"
#define SELECT_TABLE_QUESTION "SELECT * FROM QUESTIONS ;"
#define SELECT_TABLE_ROOM "SELECT * FROM ROOMS ;"
#define SELECT_ROOM_ISABLE "SELECT id_room FROM ROOM WHERE  "
#define UPDATE_USER_JOIN_ROOM "UPDATE ROOMS SET user = '%s' WHERE NAMEROOM = '%s';"
#define UPDATE_STATUS_ROOM "update ROOMS set status = '%d' where nameroom = '%s' ;"
#define GET_TIME_DURATIION "SELECT timeduration FROM ROOMS WHERE NAMEROOM = '%s';"
#define UPDATE_TIME_DURATION "UPDATE ROOMS SET TIMEDURATION = '%d' WHERE NAMEROOM ='%s' ;"




