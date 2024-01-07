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
                                    "correct_answer TEXT,"\
                                    "topic TEXT,"\
                                    "level_question INT);"
// #define CREATE_ROOM_TABLE_SQL    "CREATE TABLE ROOMS (" \
//                                     "id_room INTEGER PRIMARY KEY AUTOINCREMENT, " \
//                                     "nameroom TEXT, " \
//                                     "status INT,"\
//                                     "timeduration INTEGER,"\
//                                     "numberofquestion INTEGER  ,"\
//                                     "user TEXT, " \
//                                     "score INTEGER, "\
//                                     "easy INTEGER, " \
//                                     "normal INTEGER, " \
//                                     "difficult INTEGER, " \
//                                     "veryhard INTEGER, " \
//                                     "topic TEXT );" 

#define CREATE_TABLE_TOTAL_ROOM    "CREATE TABLE TOTALROOM (" \
                                        "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                                        "STATUS INT , "\
                                        "timeduration INT,"\
                                        "EASY INT ,"\
                                        "NORMAL INT ,"\
                                        "DIFFICULT INT , "\
                                        "VERYHARD INT ,"\
                                        "TOPIC TEXT ," \                                     
                                        "ROOMNAME VARCHAR(255) NOT NULL UNIQUE);  "

#define  CREATE_INFO_ROOM       "CREATE TABLE INFOROOM ( "\
                                "id INTEGER PRIMARY KEY AUTOINCREMENT,"\
                                "id_total INT,"\
                                "user TEXT, " \
                                "score INTEGER, "\
                                "FOREIGN KEY (id_total) REFERENCES TOTALROOM(id) );"



#define INSERT_USER "INSERT INTO USERS \
        (username, password) VALUES ('%s', '%s');"

#define INSERT_QUESTION "INSERT INTO QUESTIONS \
                                (question, answer_a, answer_b, answer_c, answer_d, correct_answer, topic, level_question ) \
                         VALUES ('%s',       '%s',      '%s',     '%s',    '%s',        '%s',       '%s',    '%d');"


#define INSERT_ROOM "INSERT INTO ROOMS \
                (nameroom, status, timeduration, numberofquestion, user, score,   easy,   normal,  difficult , veryhard,  topic ) \
        VALUES (    '%s',   '%d',    '%d',        '%d',            '%s',  '%d',   '%d',   '%d',       '%d' ,     '%d',     '%s');"



#define INSERT_TOTAL_ROOM "INSERT INTO TOTALROOM \
                (STATUS, timeduration, EASY,   NORMAL,  DIFFICULT , VERYHARD,  TOPIC , ROOMNAME) \
        VALUES (  '%d',    '%d',       '%d',   '%d',       '%d' ,     '%d',     '%s',    '%s');"

#define SQL_SELECT_ID_USER "SELECT id_user FROM USERS WHERE username = '%s' AND password = '%s';"
#define SELECT_TABLE_QUESTION "SELECT * FROM QUESTIONS ;"
#define SELECT_TABLE_ROOM "SELECT * FROM TOTALROOM ;"
// #define SELECT_ROOM_ISABLE "SELECT id_room FROM ROOM WHERE  "
#define UPDATE_USER_JOIN_ROOM "INSERT INTO  INFOROOM (id_total, user) VALUES ('%d', '%s');"
#define UPDATE_SCORE_JOIN_ROOM "INSERT INTO  INFOROOM (id_total, user, SCORE) VALUES ('%d', '%s', '%d');"

#define UPDATE_STATUS_ROOM "update ROOMS set status = '%d' where nameroom = '%s' ;"
#define GET_TIME_DURATIION "SELECT timeduration FROM ROOMS WHERE NAMEROOM = '%s';"
#define UPDATE_TIME_DURATION "UPDATE TOTALROOM SET TIMEDURATION = '%d' WHERE ROOMNAME ='%s' ;"
#define SELECT_TABLE_USERS "SELECT * FROM USERS"
#define UPDAT_SCORE "UPDATE ROOMS SET SCORE = '%d'  WHERE NAMEROOM = '%s' ;"
#define INSERT_NAME_TOTAL_ROOM  "INSERT INTO TOTALROOM (ROOMNAME) VALUES ('%s');"
// #define INSERT_ALL_TOTALROOM ""

#define SELECT_ALL_ROOM "SELECT * FROM TOTALROOM; "

#define SELECT_INFO_ROOM "SELECT * FROM '%s' ;"

#define SELECT_ID_TOTAL_ROOM  "SELECT ID FROM TOTALROOM WHERE ROOMNAME = '%s';"

#define INSERT_USER_INTO_INFOROOM "INSERT INTO INFOROOM (id_total,  user , SCORE) VALUES ('%d', '%s' , '%d');"

#define SELECT_ALL_INFOROOM "SELECT * FROM INFOROOM ;"

#define SELECT_NAME_ROOM_FROM_ID "SELECT ROOMNAME FROM TOTALROOM WHERE ID = '%d' ;"

