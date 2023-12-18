#include "SqliteDatabase.h"
#include "SqliteDatabase.h"
// std::string dbPath = "users.db"; 


void SqliteDatabase::initializeDatabase() 
{
    dbPath =  "users.db"; 
    int rc = sqlite3_open(databaseName, &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }

    //Create tables
    rc = sqlite3_exec(db, CREATE_USER_TABLE_SQL, NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't create table user: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
    rc = sqlite3_exec(db, CREATE_USER_QUESTION_SQL, NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't create table question: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }
}
bool SqliteDatabase::inserIntoTableUser(std::string username, std::string pw)
{
    int rc = sqlite3_open(dbPath.c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string sql;
    char query[512];
    snprintf(query, sizeof(query), INSERT_USER, username.c_str(), pw.c_str());
    sql += query;
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
    if (rc != SQLITE_OK) {
        std::cerr << "Can't inserIntoTableUser : " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
        return true;
    }
    return false;
}
int SqliteDatabase::getIdUser(std::string name, std::string pw)
{
    int rc = sqlite3_open(dbPath.c_str(), &db);
    int id = -1;  // Default value if not found
    std::string sql;
    char query[256];
    snprintf(query, sizeof(query), SQL_SELECT_ID, name.c_str(), pw.c_str());
    sql += query;
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
    if (rc != SQLITE_OK) 
    {
        std::cerr << "getIdUser: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(EXIT_FAILURE);
        return id;
    }
    return rc;
}
std::vector<Question> SqliteDatabase::getQuestionTraining()
{
    
}
