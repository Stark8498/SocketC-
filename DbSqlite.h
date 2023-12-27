#pragma once 
#include <sqlite3.h>
#include "db_typedef.h"
#include "DbSqlite.h"

class DbSqlite
{
    public:
        ~DbSqlite();
        static DbSqlite* getInstance();

    public:
        bool insert_user_data(User &userinfor);
        bool insert_question_data(Question &question);
        bool insert_room_data(Room &room);
        std::string get_path_file_limit();
        int search_id_user(User &user);
        bool update_status_room(std::string nameRoom);
        bool update_user_room(std::string nameUser, std::string nameRoom);
        bool get_timeDuration(std::string nameRoom, int &timeDuration);
        bool update_timeDuration(Room &room);
        bool get_room_info(std::vector <Room> &room);
        bool get_user_info(std::vector<User> &user);
        bool get_question_info(std::vector<Question> &question);


    private:
        sqlite3 *db;
        bool db_ready;
        std::vector<std::string> tableCreateQuery;
        DbSqlite();
        bool create_db();
        bool close_db();
};
