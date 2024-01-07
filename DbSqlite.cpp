#include "DbSqlite.h"

static DbSqlite *mInstancePtr = nullptr;
bool DbSqlite::close_db()
{
    int ret;
    // permanently save data
    ret = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
    if (ret != SQLITE_BUSY)
    {
        fprintf(stdout, "Permanently save all failed changes"); // luu khong thanh cong co lam gi tiep khong
    }

    ret = sqlite3_close(db);
    if (ret != SQLITE_OK)
    {
        fprintf(stdout, "Sqlite close fail");
        return false;
    }
    return true;
}

/**
 *@ ********************************************************************
 *@ Name           : exists_open                                       *
 *@ Description    : check file database exist                         *
 *@ Arguments      : name[IN]: file database                           *
 *@ Return Value   : true/false                                        *
 *@ Notes          :                                                   *
 *@ Change History :                                                   *
 *@ ********************************************************************
 **/
inline bool exists_open(const std::string &name)
{
    if (FILE *file = fopen(name.c_str(), "r"))
    {
        fclose(file);
        return true;
    }

    return false;
}

/**
 *@ ********************************************************************
 *@ Name           : constructor                                       *
 *@ Description    : create table for database                         *
 *@ Arguments      : N/A                                               *
 *@ Return Value   : N/A                                               *
 *@ Notes          :                                                   *
 *@ Change History :                                                   *
 *@ ********************************************************************
 **/
DbSqlite::DbSqlite()
{
    char sql[MAX_SIZE];
    memset(sql, 0, MAX_SIZE);
    tableCreateQuery.clear();
    tableCreateQuery.push_back(CREATE_USER_TABLE_SQL);
    tableCreateQuery.push_back(CREATE_USER_QUESTION_SQL);
    tableCreateQuery.push_back(CREATE_TABLE_TOTAL_ROOM);
    tableCreateQuery.push_back(CREATE_INFO_ROOM);

    db_ready = create_db();
    
}

/**
 *@ ********************************************************************
 *@ Name           : destructor                                        *
 *@ Description    : close a database connection                       *
 *@ Arguments      : N/A                                               *
 *@ Return Value   : N/A                                               *
 *@ Notes          :                                                   *
 *@ Change History :                                                   *
 *@ ********************************************************************
 **/
DbSqlite::~DbSqlite()
{
    close_db();
}

/**
 *@ ********************************************************************
 *@ Name           : getInstance                                       *
 *@ Description    : create singleton object                           *
 *@ Arguments      : N/A                                               *
 *@ Return Value   : only object for class DbSqlite                    *
 *@ Notes          :                                                   *
 *@ Change History :                                                   *
 *@ ********************************************************************
 **/
DbSqlite *DbSqlite::getInstance()
{
    if (mInstancePtr == nullptr)
    {
        mInstancePtr = new DbSqlite();
    }
    return mInstancePtr;
}

/**
 *@ ********************************************************************
 *@ Name           : create_db                                         *
 *@ Description    : open or creat new database                        *
 *@ Arguments      : N/A                                               *
 *@ Return Value   : true/false                                        *
 *@ Notes          :                                                   *
 *@ Change History :                                                   *
 *@ ********************************************************************
 **/
/**
 * The function `create_db` checks if a database file exists, opens the database, and creates tables if
 * the database does not exist.
 *
 * @return a boolean value.
 */
bool DbSqlite::create_db()
{
    int ret;

    bool isExist = exists_open(DB_FILE_NAME);

    // open db
    ret = sqlite3_open(DB_FILE_NAME, &db);
    if (ret)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }

    // exist db, no do anything & exit
    if (isExist)
    {
        fprintf(stderr, "db is alraedy exist, exit!!\n");
        return true;
    }

    for (int i = 0; i < tableCreateQuery.size(); i++)
    {
        char *zErrMsg = 0;
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        printf("%d [F LOG] %s create table: %s\n",
               __LINE__, __PRETTY_FUNCTION__,
               tableCreateQuery[i].c_str());

        // snprintf(sql, MAX_SIZE,  tableCreateQuery[i].c_str());
        snprintf(sql, MAX_SIZE, "%s", tableCreateQuery[i].c_str());

        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if (ret != SQLITE_OK)
        {
            // printf("%d [F LOG] %s tableInfo[%d]: %s SQL error: %s\n",
            //        __LINE__, __PRETTY_FUNCTION__,
            //        tableCreateQuery[i].c_str(), i, zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    return true;
}

/**
 * The function `insert_user_data` inserts user information into a SQLite database and returns true if
 * successful.
 *
 * @param userinfor The parameter `userinfor` is an object of type `User` that contains the user
 * information to be inserted into the database. It likely has properties such as `username` and
 * `password` that are used to construct the SQL query for inserting the user data into the database.
 *
 * @return a boolean value. If the insertion of user data is successful, it returns true. Otherwise, it
 * returns false.
 */
bool DbSqlite::insert_user_data(User &userinfor)
{
    int ret;
    std::cout << __LINE__ << "\n";
    if (db_ready)
    {
        std::cout << __LINE__ << "\n";
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_USER,
                 userinfor.username,
                 userinfor.password);
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__,
            //  __PRETTY_FUNCTION__, ret, zErrMsg);

            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }
    return false;
}
/**
 * The function `insert_total_room` inserts a room name into a SQLite database and returns true if
 * successful.
 *
 * @param roomName The parameter `roomName` is a string that represents the name of the room to be
 * inserted into the database.
 *
 * @return a boolean value.
 */
bool DbSqlite::insert_total_room(std::string roomName)
{
    int ret;
    std::cout << __LINE__ << "\n";
    if (db_ready)
    {
        std::cout << __LINE__ << "\n";
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_TOTAL_ROOM, roomName);
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__,
            //  __PRETTY_FUNCTION__, ret, zErrMsg);

            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert total room error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
        return true;
    }
    return false;
}
/**
 * The function `insert_question_data` inserts question data into an SQLite database and returns a
 * boolean indicating whether the operation was successful.
 *
 * @param question The parameter "question" is a reference to an object of type "Question". It contains
 * the data for a single question, including its content, choices, correct answer, topic, and level.
 *
 * @return a boolean value.
 */
bool DbSqlite::insert_question_data(Question &question)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_QUESTION,
                 question.content,
                 question.choices1,
                 question.choices2,
                 question.choices3,
                 question.choices4,
                 question.correctAnswer,
                 question.topic,
                 question.level);
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__,
            // __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL insert_question_data error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}
/* return 0 == OK, return 1 == no insert , return 2 == nameroom existed*/
/**
 * The function `insert_room_data` inserts room data into a SQLite database.
 *
 * @param room The parameter "room" is of type "Total_room", which is a user-defined structure or
 * class. It contains various attributes such as "name", "timeDuration", "numberQuestion", "status",
 * "score", and "user". These attributes are used to populate the SQL query for inserting data
 *
 * @return an integer value. The possible return values are:
 * - 0: If the insertion operation was successful.
 * - 1: If there was an error during the insertion operation.
 * - 2: If there was a constraint violation during the insertion operation.
 */
int DbSqlite::insert_room_data(Total_room &room)
{
    std::cout << __LINE__ << __FUNCTION__ 
    << "|room topic: " << room.topic  << std::endl;

    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        // std::cout << __LINE__ << std::endl;
        // std::cout << room.name << " " << room.timeDuration << " " << room.numberQuestion << ""
        //           << room.status << " " << room.score << " " << room.user;
        snprintf(sql, MAX_SIZE, INSERT_TOTAL_ROOM,
                 room.status,
                 room.timeDuration,
                 room.easy,
                 room.normal,
                 room.difficult,
                 room.veryhard,
                 room.topic,
                 room.name);
        std::cout << __LINE__ << sql << std::endl;

        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        std::cout << __LINE__ << std::endl;

        if (ret == SQLITE_CONSTRAINT)
        {
            return ret = 2;
        }
        else if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__,
            //  __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert total room error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return 1;
        }
    }
    return 0;
}
int DbSqlite::search_id_user(User &user)
{
    if (db_ready)
    {
        sqlite3_stmt *stmt;
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        sprintf(sql, SQL_SELECT_ID_USER,
                user.username,
                user.password);

        // Check if path exists in database
        int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (ret != SQLITE_OK)
        {
            std::cerr << "Error search_id_user: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return -1;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // imgPath exists in database, return true
            user.id = sqlite3_column_int(stmt, 1);
            sqlite3_finalize(stmt);
            return user.id;
        }
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            ret = -1;
            return ret;
        }
    }
    return 0;
}

bool DbSqlite::get_question_info(std::vector<Question> &question)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    if (db_ready)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        sqlite3_stmt *stmt;
        int ret = sqlite3_prepare_v2(db, SELECT_TABLE_QUESTION, -1, &stmt, NULL);
        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get data from QUESTIONS error: %s\n");
            sqlite3_free(stmt);
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Question questionInfo;

            questionInfo.id = sqlite3_column_int(stmt, 0);
            char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 1)));
            strncpy(questionInfo.content, tmp, 99);
            questionInfo.content[99] = '\0';
            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 2)));
            strncpy(questionInfo.choices1, tmp, 99);
            questionInfo.choices1[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 3)));
            strncpy(questionInfo.choices2, tmp, 99);
            questionInfo.choices2[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 4)));
            strncpy(questionInfo.choices3, tmp, 99);
            questionInfo.choices3[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 5)));
            strncpy(questionInfo.choices4, tmp, 99);
            questionInfo.choices4[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 6)));
            strncpy(questionInfo.correctAnswer, tmp, 99);
            questionInfo.correctAnswer[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 7)));
            strncpy(questionInfo.topic, tmp, 99);
            questionInfo.correctAnswer[99] = '\0';

            questionInfo.level = sqlite3_column_int(stmt, 8);

            question.push_back(questionInfo);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}

bool DbSqlite::get_room_info(std::vector<Total_room> &room)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    if (db_ready)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
        sqlite3_stmt *stmt;
        int ret = sqlite3_prepare_v2(db, SELECT_TABLE_ROOM, -1, &stmt, NULL);
        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get data from get_room_info error: %s\n");
            sqlite3_free(stmt);
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Total_room roominfo;
            roominfo.id = sqlite3_column_int(stmt, 0);
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

            roominfo.status = sqlite3_column_int(stmt, 1);
            roominfo.timeDuration = sqlite3_column_int(stmt, 2);
            char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 8)));
            strncpy(roominfo.name, tmp, 99);
            roominfo.name[99] = '\0';
            roominfo.easy = sqlite3_column_int(stmt, 3);
            roominfo.normal = sqlite3_column_int(stmt, 4);
            roominfo.difficult = sqlite3_column_int(stmt, 5);
            roominfo.veryhard = sqlite3_column_int(stmt, 6);
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 7)));
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
            if (tmp != nullptr)
            {
                strncpy(roominfo.topic, tmp, 99);
                roominfo.topic[99] = '\0';
            }
            else
            {
                return false;
            }
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

            std::cout << "|roominfo. name: " << roominfo.name
                      << "; "
                      << "|roominfo.status: " << roominfo.status << std::endl;

            // delete tmp;
            room.push_back(roominfo);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}

bool DbSqlite::get_info_inforoom(std::vector<Room_info> &room)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    if (db_ready)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
        sqlite3_stmt *stmt;
        int ret = sqlite3_prepare_v2(db, SELECT_ALL_INFOROOM, -1, &stmt, NULL);
        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get data from QUESTIONS error: %s\n");
            sqlite3_free(stmt);
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            Room_info roominfo;
            roominfo.id = sqlite3_column_int(stmt, 0);
            roominfo.id_totalroom = sqlite3_column_int(stmt, 1);

            char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 2)));
            strncpy(roominfo.user, tmp, 99);
            roominfo.user[99] = '\0';
            roominfo.score = sqlite3_column_int(stmt, 3);

            room.push_back(roominfo);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}

std::string DbSqlite::get_namemroom_from_id(int id)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    if (db_ready)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
        sqlite3_stmt *stmt;
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        snprintf(sql, MAX_SIZE, SELECT_NAME_ROOM_FROM_ID,
                 id);
        int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get data from inforoom with id error: %s\n");
            sqlite3_free(stmt);
            return " ";
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
            char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 8)));
            if (tmp == nullptr)
            {
                sqlite3_finalize(stmt);
                return " ";
            }

            std::string ret(tmp);
            return ret;
        }
        sqlite3_free(stmt);

        return " ";
    }

    return " ";
}
// bool DbSqlite::get_struct_totalroom(std::vector<Total_room> &totalroom)
// {
//     std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

//     if (db_ready)
//     {
//         std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
//         sqlite3_stmt *stmt;
//         int ret = sqlite3_prepare_v2(db, SELECT_ALL_ROOM, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from QUESTIONS error: %s\n");
//             sqlite3_free(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             Total_room roominfo;
//             std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
//             roominfo.id = sqlite3_column_int(stmt, 0);
//             char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 1)));
//             strncpy(roominfo.roomName, tmp, 99);
//             roominfo.roomName[99] = '\0';
//             if (tmp != nullptr)
//             {
//                 strncpy(roominfo.roomName, tmp, 99);
//                 roominfo.roomName[99] = '\0';
//             }
//             else
//             {
//                 return false;
//             }
//             std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

//             // delete tmp;
//             totalroom.push_back(roominfo);
//         }
//         sqlite3_free(stmt);

//         return true;
//     }

//     return false;
// }

// bool DbSqlite::get_struct_info_room(std::string nameRoom, std::vector<Room_info> &room)
// {
//     std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

//     if (db_ready)
//     {
//         std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
//         sqlite3_stmt *stmt;
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         snprintf(sql, MAX_SIZE, SELECT_INFO_ROOM,
//                 nameRoom.c_str() );
//         int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from QUESTIONS error: %s\n");
//             sqlite3_free(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//            Room_info roominfo;
//             std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
//             roominfo.id = sqlite3_column_int(stmt, 0);
//             roominfo.id_totalroom = sqlite3_column_int(stmt, 1);
//             roominfo.status = sqlite3_column_int(stmt, 2);
//             roominfo.timeDuration = sqlite3_column_int(stmt, 3);
//             roominfo.numberQuestion = sqlite3_column_int(stmt, 4);

//             char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 5)));
//             strncpy(roominfo.user, tmp, 99);
//             roominfo.user[99] = '\0';

//             roominfo.score = sqlite3_column_int(stmt, 6);

//             roominfo.easy = sqlite3_column_int(stmt, 7);
//             roominfo.normal = sqlite3_column_int(stmt, 8);
//             roominfo.difficult = sqlite3_column_int(stmt, 9);
//             roominfo.veryhard = sqlite3_column_int(stmt, 10);

//             tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 11)));
//             std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
//             if (tmp != nullptr)
//             {
//                 strncpy(roominfo.topic, tmp, 99);
//                 roominfo.topic[99] = '\0';
//             }
//             else
//             {
//                 return false;
//             }
//             std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

//             std::cout << "|roominfo. name: " << nameRoom
//                       << "; "
//                       << "|roominfo.status: " << roominfo.status << std::endl;

//             // delete tmp;
//             room.push_back(roominfo);
//         }
//         sqlite3_free(stmt);

//         return true;
//     }

//     return false;
// }

bool DbSqlite::update_status_room(std::string nameRoom)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, UPDATE_STATUS_ROOM,
                 2,
                 nameRoom.c_str());
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}
bool DbSqlite::update_user_room(std::string nameUser, std::string nameRoom)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, UPDATE_USER_JOIN_ROOM,
                 nameUser.c_str(),
                 nameRoom.c_str());
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}
bool DbSqlite::get_timeDuration(std::string nameRoom, int &timeDuration)
{
    if (db_ready)
    {
        sqlite3_stmt *stmt;
        int ret = sqlite3_prepare_v2(db, GET_TIME_DURATIION, -1, &stmt, NULL);

        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get time duration from  room error: %s\n");
            sqlite3_free(stmt);
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            timeDuration = sqlite3_column_int(stmt, 3);
            sqlite3_free(stmt);
        }
        return true;
    }
    return false;
}

// int DbSqlite::get_id_from_totalroom(std::string nameRoom)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         int ret = sqlite3_prepare_v2(db, SELECT_ID_TOTAL_ROOM, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get ID FROM TOTALROOM error: %s\n");
//             sqlite3_free(stmt);
//             return false;
//         }

//         if (ret == SQLITE_ROW)
//         {
//             int roomId = sqlite3_column_int(stmt, 0);
//             sqlite3_finalize(stmt);
//             sqlite3_close(db);
//             return roomId;
//         }
//         else if (ret == SQLITE_DONE)
//         {
//             // Không có dòng nào khớp với điều kiện WHERE
//             return 0;
//             std::cout << "Room not found." << std::endl;
//         }
//         else
//         {
//             // Có lỗi xảy ra
//             std::cerr << "Error executing query: " << sqlite3_errmsg(db) << std::endl;
//         }
//         return 2;
//     }
//     return 0;
// }

int DbSqlite::get_id_total_room(std::string nameRoom)
{
    if (db_ready)
    {
        sqlite3_stmt *stmt;
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        sprintf(sql, SELECT_ID_TOTAL_ROOM,
                nameRoom.c_str());

        // Check if path exists in database
        int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (ret != SQLITE_OK)
        {
            std::cerr << "Error search_event_priority: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return 0;
        }
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // imgPath exists in database, return true
            int id = sqlite3_column_int(stmt, 0);
            sqlite3_finalize(stmt);
            return id;
        }
        if (sqlite3_step(stmt) == SQLITE_DONE)
        {
            ret = 0;
            return ret;
        }
    }
    return 0;
}

bool DbSqlite::update_timeDuration(Total_room &room)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, UPDATE_TIME_DURATION,
                 room.timeDuration,
                 room.name);
        std::cout << __LINE__ << "|SQL: " << sql << std::endl;

        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user update time duraiton error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}

bool DbSqlite::insert_userscore_inforoom(int id, std::string user, int score)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_USER_INTO_INFOROOM,
                 id,
                 user.c_str(),
                 score);
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if (ret == SQLITE_CONSTRAINT)
        {
            std::cout << "id room is not exist\n";
            return false;
        }
        else if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user update time duraiton error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}

bool DbSqlite::get_user_info(std::vector<User> &user)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    if (db_ready)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
        sqlite3_stmt *stmt;
        int ret = sqlite3_prepare_v2(db, SELECT_TABLE_USERS, -1, &stmt, NULL);
        if (ret != SQLITE_OK)
        {
            fprintf(stderr, "Get data from USERS error: %s\n");
            sqlite3_free(stmt);
            return false;
        }

        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            User userinfo;
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
            userinfo.id = sqlite3_column_int(stmt, 0);
            char *tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 1)));
            if (tmp == nullptr)
            {
                return false;
            }

            strncpy(userinfo.username, tmp, 99);
            userinfo.username[99] = '\0';

            tmp = reinterpret_cast<char *>(const_cast<unsigned char *>(sqlite3_column_text(stmt, 1)));
            if (tmp == nullptr)
            {
                return false;
            }
            strncpy(userinfo.password, tmp, 99);
            userinfo.password[99] = '\0';

            // delete tmp;
            user.push_back(userinfo);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}
bool DbSqlite::set_score_room(int score, std::string nameRoom)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, UPDAT_SCORE,
                 score,
                 nameRoom.c_str());
        std::cout << __LINE__ << " : " << __FUNCTION__ << sql << std::endl;

        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user update score room error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}

bool DbSqlite::creat_room_info(char roomName[100])
{
    sqlite3 *db;
    int rc = sqlite3_open(DB_FILE_NAME, &db);
    if (rc)
    {
        std::cerr << "Cannot open database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    char sql[MAX_SIZE];
    memset(sql, 0, MAX_SIZE);
    char *zErrMsg = 0;
    snprintf(sql, MAX_SIZE, CREATE_INFO_ROOM,
             roomName);
    rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        std::cerr << "Cannot create room info table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }
    sqlite3_close(db);

    std::cout << "Tables created successfully." << std::endl;

    return true;
}
// bool DbSqlite::insert_room_info(char nameRoom[100] , Room_info room)
// {
//     std::cout << __LINE__ << __FUNCTION__ << std::endl;

//     int ret;
//     if (db_ready)
//     {
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         char *zErrMsg = 0;

//         // const int ID_USER = 1;
//         // std::cout << __LINE__ << std::endl;
//         // std::cout << room.name << " " << room.timeDuration << " " << room.numberQuestion << ""
//         //           << room.status << " " << room.score << " " << room.user;
//         snprintf(sql, MAX_SIZE, INSERT_ROOM,
//                  nameRoom,
//                  room.status,
//                  room.timeDuration,
//                  room.numberQuestion,
//                  room.user,
//                  room.score,
//                  room.easy,
//                  room.normal,
//                  room.difficult,
//                  room.veryhard,
//                  room.topic);
//         std::cout << __LINE__ << sql << std::endl;

//         ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
//         std::cout << __LINE__ << std::endl;

//         if (ret != SQLITE_OK)
//         {
//             char temp[1000] = {0};
//             memset(temp, 0, 1000);
//             // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__,
//             //  __PRETTY_FUNCTION__, ret, zErrMsg);
//             // printf("Insert_To_Data_Base sql = %s\n", sql);
//             fprintf(stderr, "SQL user insert roominfo error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }
//     return true;
// }