#pragma once
#include "common.h"
#include <sqlite3.h>
#include "db_typedef.h"
#include "DbSqlite.h"

class DbSqlite
{
public:
    ~DbSqlite();
    static DbSqlite *getInstance();

public:
    bool insert_user_data(User &userinfor);
    bool insert_question_data(Question &question);
    int insert_room_data(Total_room &room);
    bool insert_total_room(std::string roomName);
    int get_id_total_room(std::string nameString);
    bool insert_room_info(char nameRoom[100], Room_info roominfo);
    bool get_info_inforoom(std::vector<Room_info> &roominfo) ;
    std::string get_path_file_limit();
    int search_id_user(User &user);
    bool update_status_room(std::string nameRoom);
    bool update_user_room(std::string nameUser, std::string nameRoom);
    bool get_timeDuration(std::string nameRoom, int &timeDuration);
    bool update_timeDuration(Total_room &room);
    bool get_room_info(std::vector<Total_room> &room);
    bool get_user_info(std::vector<User> &user);
    bool get_question_info(std::vector<Question> &question);
    bool set_score_room(int score, std::string nameRoom);
    bool creat_room_info(char roomName[100]);
    bool insert_userscore_inforoom(int id, std::string user, int score);
    std::string  get_namemroom_from_id(int id);

private:
    sqlite3 *db;
    bool db_ready;
    std::vector<std::string> tableCreateQuery;
    DbSqlite();
    bool create_db();
    bool close_db();
};
