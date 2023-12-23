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
    tableCreateQuery.push_back(CREATE_ROOM_TABLE_SQL);

    db_ready = create_db();

    // MyController::getInstance()->set_event_status_authen(check_camera_authen_status());
    // MyController::getInstance()->set_mcc_service_enable(check_mcc_enable_status());
    // MyController::getInstance()->set_secondary_url_enable(check_secondary_url_enable_status());
    // MyController::getInstance()->set_device_registation_status(check_device_registation_status());

    // printf("%d [F LOG] %s admccEna: %d | secondURLEna: %d | deviceRegStatus: %d\n",
    //        __LINE__, __PRETTY_FUNCTION__,
    //        MyController::getInstance()->get_mcc_service_enable(),
    //        MyController::getInstance()->get_secondary_url_enable(),
    //        MyController::getInstance()->get_device_registation_status());
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
        std::cout << __LINE__  << ": " << __FUNCTION__ << "\n";

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
                 question.correctAnswer);
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if (ret != SQLITE_OK)
        {
            char temp[1000] = {0};
            memset(temp, 0, 1000);
            // sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, 
            // __PRETTY_FUNCTION__, ret, zErrMsg);
            // printf("Insert_To_Data_Base sql = %s\n", sql);
            fprintf(stderr, "SQL user insert error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
            return false;
        }
    }
    return true;
}

bool DbSqlite::insert_room_data(Room &room)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        std::cout << __LINE__ << std::endl;
        std::cout << room.name << " " << room.timeDuration << " " <<  room.numberQuestion << "" 
        << room.status << " " << room.score << " " << room.user; 
        snprintf(sql, MAX_SIZE, INSERT_ROOM,
                 room.name,
                 room.status,
                 room.timeDuration,
                 room.numberQuestion,
                 room.user,
                 room.score);
    std::cout << __LINE__ << std::endl;

        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
    std::cout << __LINE__ << std::endl;


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
    }
    return true;
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
    if (db_ready)
    {
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
            char *tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            strncpy(questionInfo.content, tmp, 99);
            questionInfo.content[99] = '\0';
            tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
            strncpy(questionInfo.choices1, tmp, 99);
            questionInfo.choices1[99] = '\0';

            tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
            strncpy(questionInfo.choices2, tmp, 99);
            questionInfo.choices2[99] = '\0';
            
            tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
            strncpy(questionInfo.choices3, tmp, 99);
            questionInfo.choices3[99] = '\0';

            tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 2)));
            strncpy(questionInfo.choices4, tmp, 99);
            questionInfo.choices4[99] = '\0';
            
            questionInfo.correctAnswer = sqlite3_column_int(stmt, 6);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}

bool DbSqlite::get_room_info(std::vector<Room> &room)
{
    if (db_ready)
    {
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
            Room roominfo;

            roominfo.id = sqlite3_column_int(stmt, 0);
            char *tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 1)));
            strncpy(roominfo.name, tmp, 99);
            roominfo.name[99] = '\0';

            roominfo.status = sqlite3_column_int(stmt, 2);
            roominfo.timeDuration = sqlite3_column_int(stmt, 3);
            roominfo.numberQuestion = sqlite3_column_int(stmt, 4);
            
            tmp = reinterpret_cast<char*>(const_cast<unsigned char*>(sqlite3_column_text(stmt, 5)));
            strncpy(roominfo.user, tmp, 99);
            roominfo.user[99] = '\0';

            roominfo.score = sqlite3_column_int(stmt, 6);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}
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
bool DbSqlite::update_timeDuration(Room &room)
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
