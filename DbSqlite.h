#pragma once 
#include <sqlite3.h>
#include "db_typedef.h"
#include "common.h"

class DbSqlite
{
    public:
        ~DbSqlite();
        static DbSqlite* getInstance();

    public:
        bool insert_user_data(User &userinfor);
        bool get_question_info(std::vector<Question> &question);
        bool insert_question_data(Question &question);
        std::string get_path_file_limit();
        int search_id_user(User &user);
        bool get_room_info(std::vector <Room> &room);
        bool insert_room_data(Room &room);


    private:
        sqlite3 *db;
        bool db_ready;
        std::vector<std::string> tableCreateQuery;
        DbSqlite();
        bool create_db();
        bool close_db();
};
