#pragma one
#include <iostream>
#include <vector>
#include "sql_common.h"
#include <sqlite3.h>
#include "common.h"

struct User {
    int id_user;
    std::string username;
    std::string password;
};
// std::string dbPath = "users.db"; 


class SqliteDatabase {
public:
    static SqliteDatabase& getInstance() 
    {
        static SqliteDatabase instance(dbPath.c_str());
        return instance;
    }

    bool inserIntoTableUser(std::string username, std::string pw);
    // bool inserIntoTableQuestion(std::vector<User> &v_question);
    int getIdUser(std::string name, std::string pw);
    void initializeDatabase();
    std::vector<Question> getQuestionTraining();


    void closeDatabase() {
        if (db) {
            sqlite3_close(db);
            db = nullptr;
        }
    }


    ~SqliteDatabase() {
        closeDatabase();
    }

private:
    SqliteDatabase(const char* dbName) : databaseName(dbPath.c_str()), db(nullptr) {}

    SqliteDatabase(const SqliteDatabase&) = delete;
    SqliteDatabase& operator=(const SqliteDatabase&) = delete;

    const char* databaseName;
    sqlite3* db;
    static std::string dbPath;
    const char* databaseName;
    sqlite3* db;
};