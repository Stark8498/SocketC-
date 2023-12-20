#include "DbSqlite.h"

static DbSqlite *mInstancePtr = nullptr;

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

        snprintf(sql, MAX_SIZE, tableCreateQuery[i].c_str());
        ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

        if (ret != SQLITE_OK)
        {
            printf("%d [F LOG] %s tableInfo[%d]: %s SQL error: %s\n",
                   __LINE__, __PRETTY_FUNCTION__,
                   tableCreateQuery[i].c_str(), i, zErrMsg);
            sqlite3_free(zErrMsg);
        }
    }

    return true;
}

bool DbSqlite::insert_user_data(User &userinfor)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_USER,
                 userinfor.username.c_str(),
                 userinfor.password.c_str());
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
                 question.content.c_str(),
                 question.choices1.c_str(),
                 question.choices2.c_str(),
                 question.choices3.c_str(),
                 question.choices4.c_str(),
                 question.correctAnswer);
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

bool DbSqlite::insert_room_data(Room &room)
{
    int ret;
    if (db_ready)
    {
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        char *zErrMsg = 0;

        // const int ID_USER = 1;
        snprintf(sql, MAX_SIZE, INSERT_QUESTION,
                 room.name.c_str(),
                 room.status,
                 room.timeDuration,
                 room.numberQuestion);
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


int DbSqlite::search_id_user(User &user)
{
    if (db_ready)
    {
        sqlite3_stmt *stmt;
        char sql[MAX_SIZE];
        memset(sql, 0, MAX_SIZE);
        sprintf(sql, SQL_SELECT_ID_USER,
                    user.username.c_str(),
                    user.password.c_str());

        // Check if path exists in database
        int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

        if (ret != SQLITE_OK)
        {
            std::cerr << "Error search_id_user: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_finalize(stmt);
            return 0;
        }       
        while (sqlite3_step(stmt) == SQLITE_ROW)
        {
            // imgPath exists in database, return true
            user.id = sqlite3_column_int(stmt, 1);
            sqlite3_finalize(stmt);
            return  user.id;
        }
        if(sqlite3_step(stmt) == SQLITE_DONE)
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
            questionInfo.content = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
            questionInfo.choices1 = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
            questionInfo.choices2 = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3)));
            questionInfo.choices3 = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
            questionInfo.choices4 = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
            questionInfo.correctAnswer = sqlite3_column_int(stmt, 6);          
            const char* columnText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            question.push_back(questionInfo);
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
            roominfo.name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
            roominfo.status = sqlite3_column_int(stmt, 2);
            roominfo.timeDuration = sqlite3_column_int(stmt, 3);
            roominfo.score = sqlite3_column_int(stmt, 4);
            roominfo.user = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
            roominfo.numberQuestion = sqlite3_column_int(stmt, 6);          
            room.push_back(roominfo);
        }
        sqlite3_free(stmt);

        return true;
    }

    return false;
}

// bool DbSqlite::insert_authen_data(AuthenticationInfo &authenInfo)
// {
//     int ret;

//     if (db_ready)
//     {
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         char *zErrMsg = 0;

//         const int ID_AUTHEN = 1;
//         snprintf(sql, MAX_SIZE, INSERT_DATA_BASE, ID_AUTHEN,
//                  authenInfo.username.c_str(),
//                  authenInfo.password.c_str(),
//                  authenInfo.access_token.c_str(),
//                  authenInfo.token_type.c_str(),
//                  authenInfo.expires_in,
//                  authenInfo.refresh_token.c_str(),
//                  authenInfo.issued.c_str(),
//                  authenInfo.expires.c_str(),
//                  authenInfo.primaryUrl.c_str(),
//                  authenInfo.secondaryUrl.c_str(),
//                  authenInfo.time_switch,
//                  authenInfo.status_authen.c_str(),
//                  authenInfo.admccEnable,
//                  authenInfo.secondaryURLEnable);
//         ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

//         if (ret != SQLITE_OK)
//         {
//             char temp[1000] = {0};
//             memset(temp, 0, 1000);
//             sprintf(temp, "[DB %d %s] ret: %d | err_msg: %s\n", __LINE__, __PRETTY_FUNCTION__, ret, zErrMsg);
//             string log = string(temp);
//             MCCLogger::getInstance()->write_log(log);

//             // printf("Insert_To_Data_Base sql = %s\n", sql);
//             fprintf(stderr, "SQL authen insert error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }
//     return true;
// }







// bool DbSqlite::get_authen_info(AuthenticationInfo *obj)
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         int ret = sqlite3_exec(db, SELECT_AUTHEN_INFO, get_data_authen, (void *)obj, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from AUTHENTICATION error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }
//     return true;
// }

// int get_data_device(void *obj, int argc, char **argv, char **azColName)
// {
//     if (obj == nullptr)
//     {
//         return 0;
//     }

//     std::string::size_type sz;
//     DeviceInfo *deviceData = (DeviceInfo *)obj;

//     deviceData->device_name = argv[1];
//     deviceData->id_device = argv[2];
//     deviceData->physical_id = argv[3];
//     deviceData->location.latitude = std::stod(argv[4], &sz);
//     deviceData->location.longitude = std::stod(argv[5], &sz);
//     deviceData->location.azimuth = std::stod(argv[6], &sz);
//     deviceData->location.height = std::stod(argv[7], &sz);
//     deviceData->status_device = argv[8];

//     return 0;
// }

// /**
//  *@ ********************************************************************
//  *@ Name           : get_device_info                                   *
//  *@ Description    : get data from the table device                    *
//  *@ Arguments      : [IN/OUT]obj                                       *
//  *@ Return Value   : true/false                                        *
//  *@ Notes          :                                                   *
//  *@ Change History :                                                   *
//  *@ ********************************************************************
//  **/
// bool DbSqlite::get_device_info(DeviceInfo *obj)
// {
//     int ret;

//     if (db_ready)
//     {
//         char *zErrMsg = 0;
//         ret = sqlite3_exec(db, SELECT_DEVICE_INFO, get_data_device, (void *)obj, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from DEVICE error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }
//     return true;
// }

// /**
//  *@ ********************************************************************
//  *@ Name           : get_alarm_type                                    *
//  *@ Description    : get data from the table alarm type                *
//  *@ Arguments      : [IN/OUT]obj                                       *
//  *@ Return Value   : true/false                                        *
//  *@ Notes          :                                                   *
//  *@ Change History :                                                   *
//  *@ ********************************************************************
//  **/
// bool DbSqlite::get_alarm_type(std::vector<AlarmType> *obj)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         int ret = sqlite3_prepare_v2(db, SELECT_ALARM_TYPE_INFO, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from ALARM_TYPE error\n");
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             int id = sqlite3_column_int(stmt, 0);
//             string name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
//             string type = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
//             obj->push_back({id, name, type});
//         }
//         sqlite3_finalize(stmt);
//     }

//     return true;
// }

// int get_data_time_alarm_type(void *obj, int argc, char **argv, char **azColName)
// {
//     if (obj == nullptr)
//     {
//         return 0;
//     }

//     std::string::size_type sz;
//     long *alarmData = (long *)obj;

//     *alarmData = std::stoi(argv[1], &sz);

//     if (alarmData == 0)
//     {
//         obj = nullptr;
//     }

//     return 0;
// }

// /**
//  *@ ********************************************************************
//  *@ Name           : get_alarm_type                                    *
//  *@ Description    : get data from the table alarm type                *
//  *@ Arguments      : [IN/OUT]obj                                       *
//  *@ Return Value   : true/false                                        *
//  *@ Notes          :                                                   *
//  *@ Change History :                                                   *
//  *@ ********************************************************************
//  **/
// bool DbSqlite::get_time_alarm_type(long *obj)
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         int ret = sqlite3_exec(db, SELECT_TIME_ALARM_TYPE_INFO, get_data_time_alarm_type, (void *)obj, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from TIME_ALARM_TYPE error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//         // Query database got result = NULL (get_data_time_alarm_type)
//         if (obj == nullptr)
//         {
//             return false;
//         }
//     }
//     return true;
// }

// /**
//  *@ ********************************************************************
//  *@ Name           : get_camera_info                                   *
//  *@ Description    : get data from the table camera info               *
//  *@ Arguments      : [IN/OUT]camInfo                                   *
//  *@ Return Value   : true/false                                        *
//  *@ Notes          :                                                   *
//  *@ Change History :                                                   *
//  *@ ********************************************************************
//  **/
// bool DbSqlite::get_camera_info(CameraInfo *camInfo)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         int ret = sqlite3_prepare_v2(db, SELECT_CAMERA_INFO, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from CAMERA_INFO error\n");
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         if (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             camInfo->username = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1)));
//             camInfo->password = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
//         }
//         sqlite3_finalize(stmt);
//     }
//     return true;
// }

// bool DbSqlite::delete_event_priority()
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         int ret = sqlite3_exec(db, DELETE_DATA_EVENT_PRIORITY, NULL, 0, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Delete event priority error: %s\n",
//                    __LINE__, __PRETTY_FUNCTION__, zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }

//     return true;
// }

// /**
//  *@ ********************************************************************
//  *@ Name           : delete_alarm_type                                *
//  *@ Description    : delete data in the table device                   *
//  *@ Arguments      : N/A                                               *
//  *@ Return Value   : true/false                                        *
//  *@ Notes          :                                                   *
//  *@ Change History :                                                   *
//  *@ ********************************************************************
//  **/
// bool DbSqlite::delete_alarm_type()
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         int ret = sqlite3_exec(db, DELETE_DATA_ALARM_TYPE, NULL, 0, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Delete alarm type error: %s\n",
//                    __LINE__, __PRETTY_FUNCTION__, zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }

//     return true;
// }

// /**
//  *@ **************************************************************************
//  *@ Name           : close_db                                                *
//  *@ Description    : permanently save data and close a database connection   *
//  *@ Arguments      : N/A                                                     *
//  *@ Return Value   : true/false                                              *
//  *@ Notes          :                                                         *
//  *@ Change History :                                                         *
//  *@ **************************************************************************
//  **/
// bool DbSqlite::close_db()
// {
//     int ret;
//     // permanently save data
//     ret = sqlite3_exec(db, "COMMIT;", NULL, NULL, NULL);
//     if (ret != SQLITE_BUSY)
//     {
//         fprintf(stdout, "Permanently save all failed changes"); // luu khong thanh cong co lam gi tiep khong
//     }

//     ret = sqlite3_close(db);
//     if (ret != SQLITE_OK)
//     {
//         fprintf(stdout, "Sqlite close fail");
//         return false;
//     }
//     return true;
// }

// int DbSqlite::get_va_event_callback(void *vaEvent, int argc, char **argv, char **azColName)
// {
//     if (vaEvent == nullptr)
//     {
//         return 0;
//     }

//     std::string::size_type sz;
//     std::vector<VAEventInfo> *eventInfo = (std::vector<VAEventInfo> *)vaEvent;

//     for (int i = 0; i < argc; i++)
//     {
//         // printf("column[%d]\"%s\": %s\n", i, azColName[i], argv[i]);
//     }

//     VAEventInfo event;
//     event.id = atoi(argv[0]);
//     event.epochTime = std::stod(argv[1], &sz);
//     event.alarmId = atoi(argv[2]);
//     event.alarmTime = argv[3];
//     event.channelId = atoi(argv[4]);
//     event.alarmTypeId = atoi(argv[5]);
//     event.priority = argv[6];
//     event.classification = argv[7];
//     event.eventMetadata = argv[8];
//     event.description = argv[9];
//     event.status = atoi(argv[10]);
//     event.imagePath = argv[11];
//     eventInfo->push_back(event);

//     return 0;
// }

// bool DbSqlite::insert_va_event(VAEventInfo &eventInfo)
// {
//     if (db_ready)
//     {
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         char *zErrMsg = 0;

//         snprintf(sql, MAX_SIZE, INSERT_VA_EVENT,
//                  eventInfo.alarmId, eventInfo.channelId, eventInfo.epochTime, eventInfo.alarmTypeId,
//                  eventInfo.alarmTime.c_str(), eventInfo.priority.c_str(), eventInfo.classification.c_str(),
//                  eventInfo.eventMetadata.c_str(), eventInfo.description.c_str(), eventInfo.status, eventInfo.imagePath.c_str());

//         int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s SQL va event insert error: %s\n",
//                    __LINE__, __PRETTY_FUNCTION__, zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//         else
//         {
//             printf("%d [F LOG] %s SQL va event insert successfully\n",
//                    __LINE__, __PRETTY_FUNCTION__);
//         }
//     }
//     return true;
// }

// bool DbSqlite::get_va_event(std::vector<VAEventInfo> *eventInfos)
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;
//         int ret = sqlite3_exec(db, SELECT_VA_EVENT, get_va_event_callback, eventInfos, &zErrMsg);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s SQL va event query error: %s\n",
//                    __LINE__, __PRETTY_FUNCTION__, zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }

//         if (eventInfos->size() <= 0)
//         {
//             return false;
//         }
//     }

//     return true;
// }

// bool DbSqlite::delete_va_event(int eventId)
// {
//     if (db_ready)
//     {
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         char *zErrMsg = 0;

//         snprintf(sql, MAX_SIZE, DELETE_VA_EVENT_ID, eventId);
//         int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s SQL va event delete id: %d | query: %s | error: %s\n",
//                    __LINE__, __PRETTY_FUNCTION__, eventId, sql, zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }

//     return true;
// }

// bool DbSqlite::check_mcc_enable_status()
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         int ret = sqlite3_prepare_v2(db, SELECT_AUTHEN_ADMCC_ENABLE, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Get admccEnable from AUTHENTICATION error\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             int mcc_status = sqlite3_column_int(stmt, 0);

//             printf("%d [F LOG] %s Get admccEnable: %d\n", __LINE__, __PRETTY_FUNCTION__, mcc_status);

//             sqlite3_finalize(stmt);
//             return mcc_status;
//         }
//         sqlite3_finalize(stmt);
//     }

//     return false;
// }

// bool DbSqlite::check_secondary_url_enable_status()
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         int ret = sqlite3_prepare_v2(db, SELECT_AUTHEN_SECONDARY_URL_ENABLE, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Get secondaryUrlEnable from AUTHENTICATION error\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             int secondary_url_status = sqlite3_column_int(stmt, 0);

//             printf("%d [F LOG] %s Get admccEnable: %d\n", __LINE__, __PRETTY_FUNCTION__, secondary_url_status);

//             sqlite3_finalize(stmt);
//             return secondary_url_status;
//         }
//         sqlite3_finalize(stmt);
//     }

//     return false;
// }

// bool DbSqlite::check_device_registation_status()
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         int ret = sqlite3_prepare_v2(db, SELECT_DEVICE_PHYSICAL_ID, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Get physical_id from DEVICE error\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) != SQLITE_ROW)
//         {
//             printf("%d [F LOG] %s Get physical_id from DEVICE no data\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }
//         sqlite3_finalize(stmt);
//     }

//     return true;
// }

// bool DbSqlite::check_camera_authen_status()
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         int ret = sqlite3_prepare_v2(db, SELECT_CAMERA_INFO, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             printf("%d [F LOG] %s Get data from CAMERA_INFO error\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) != SQLITE_ROW)
//         {
//             printf("%d [F LOG] %s Get data from CAMERA_INFO no data\n", __LINE__, __PRETTY_FUNCTION__);
//             sqlite3_finalize(stmt);
//             return false;
//         }
//         sqlite3_finalize(stmt);
//     }

//     return true;
// }

// /**
//  *@ **************************************************************************
//  *@ Name           : get_timestamp_event                                     *
//  *@ Description    : check EpochTime exists in database                      *
//  *@ Arguments      : [IN] BackupEventData *obj                               *
//  *@ Return Value   : true/false                                              *
//  *@ Notes          :                                                         *
//  *@ Change History :                                                         *
//  *@ **************************************************************************
//  **/
// bool DbSqlite::get_timestamp_event(BackupEventData &obj)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;

//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         string imagePath = "channel" + std::to_string(obj.channelId) + "/" + std::to_string(obj.epochTime);
//         snprintf(sql, MAX_SIZE, SELECT_IMAGEPATH_EXIST, imagePath.c_str());

//         int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get EpochTime from VA_EVENT error\n");
//             sqlite3_finalize(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             sqlite3_finalize(stmt);
//             return false;
//         }
//         sqlite3_finalize(stmt);
//     }

//     return true;
// }

// /**
//  *@ **************************************************************************
//  *@ Name           : save_path_file                                          *
//  *@ Description    : save eventID and imgPath in database                    *
//  *@ Arguments      : [IN] int *id, string imgPath                            *
//  *@ Return Value   : true/false                                              *
//  *@ Notes          :                                                         *
//  *@ Change History :                                                         *
//  *@ **************************************************************************
//  **/
// bool DbSqlite::save_path_file(int &id, std::string &imgPath)
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);

//         snprintf(sql, MAX_SIZE, UPDATE_VA_EVENT_IMAGE_PATH, imgPath.c_str(), id);
//         int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "SQL va event update error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }
//     return true;
// }

// /**
//  *@ **************************************************************************
//  *@ Name           : delete_path                                             *
//  *@ Description    : delete path when push FE Server success                 *
//  *@ Arguments      : [IN] string imgPath, int channel_ID                     *
//  *@ Return Value   : N/A                                                     *
//  *@ Notes          :                                                         *
//  *@ Change History :                                                         *
//  *@ **************************************************************************
//  **/
// bool DbSqlite::check_image_path_exist(std::string imgPath)
// {
//     if (db_ready)
//     {
//         if (imgPath.c_str() != nullptr && imgPath != "null")
//         {
//             sqlite3_stmt *stmt;
//             char sql[MAX_SIZE];
//             memset(sql, 0, MAX_SIZE);
//             sprintf(sql, SELECT_ID_EXIST, imgPath.c_str());

//             // Check if path exists in database
//             int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

//             if (ret != SQLITE_OK)
//             {
//                 std::cerr << "Error preparing SQLite3 query: " << sqlite3_errmsg(db) << std::endl;
//                 sqlite3_finalize(stmt);
//                 return false;
//             }

//             while (sqlite3_step(stmt) == SQLITE_ROW)
//             {
//                 // imgPath exists in database, return true
//                 sqlite3_finalize(stmt);
//                 return true;
//             }

//             sqlite3_finalize(stmt);
//             return false;
//         }
//     }
//     return false;
// }

// /**
//  *@ **************************************************************************
//  *@ Name           : get_path_file_limit                                     *
//  *@ Description    : get ImagePath min                                       *
//  *@ Arguments      : N/A                                                     *
//  *@ Return Value   : path file min                                           *
//  *@ Notes          :                                                         *
//  *@ Change History :                                                         *
//  *@ **************************************************************************
//  **/
// std::string DbSqlite::get_path_file_limit()
// {
//     std::string path_file = "";

//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         double epoch_time = 0;

//         // get EpochTime min
//         int ret = sqlite3_prepare_v2(db, SELECT_VA_EVENT_MIN_EPOCHTIME, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Select Min EpochTime from VA_EVENT error\n");
//             sqlite3_finalize(stmt);
//             return nullptr;
//         }

//         if (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             epoch_time = sqlite3_column_double(stmt, 0);
//         }

//         // get ImagePath min based EpochTime min
//         memset(sql, 0, MAX_SIZE);
//         snprintf(sql, MAX_SIZE, SELECT_VA_EVENT_MIN_IMAGE_PATH, epoch_time);

//         ret = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Select Min EpochTime from VA_EVENT error\n");
//             sqlite3_finalize(stmt);
//             return nullptr;
//         }

//         if (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             path_file = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0)));
//         }

//         sqlite3_finalize(stmt);
//     }

//     return path_file;
// }

// bool DbSqlite::delete_va_event_with_image_path(std::string &imgPath)
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         snprintf(sql, MAX_SIZE, DELETE_VA_EVENT_WITH_IMAGE_PATH, imgPath.c_str());

//         int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Delete VA_EVENT with imagePath: %s error: %s\n", 
//                     imgPath.c_str(), zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }

//     return true;
// }

// int DbSqlite::search_enable_event(EventPriority &priorityInfo)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         char sql[MAX_SIZE];
//         memset(sql, 0, MAX_SIZE);
//         sprintf(sql, SELECT_INFO_ENABLE_EVENT_PRIORITY,
//                     priorityInfo.event_name.c_str(),
//                     priorityInfo.channelid,
//                     priorityInfo.zoneid.c_str());

//         // Check if path exists in database
//         int ret = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

//         if (ret != SQLITE_OK)
//         {
//             std::cerr << "Error search_event_priority: " << sqlite3_errmsg(db) << std::endl;
//             sqlite3_finalize(stmt);
//             return 0;
//         }       
//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             // imgPath exists in database, return true
//             priorityInfo.is_enable = sqlite3_column_int(stmt, 1);
//             sqlite3_finalize(stmt);
//             return priorityInfo.is_enable;
//         }
//         if(sqlite3_step(stmt) == SQLITE_DONE)
//         {
//             ret = 2;
//             return ret;
//         }
      
//     }
//     return 0;
// }


// bool DbSqlite::insert_event_priority_pre(EventPriority &priorityInfo)
// {
//     if (db_ready)
//     {
//         int ret = search_enable_event(priorityInfo);

//         // std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//         //           << " | ret = " << ret
//         //           << " search id: " << ret
//         //           << " | event_name: " <<  priorityInfo.event_name
//         //           << " | channelId: " << priorityInfo.channelid
//         //           << " | zoneId: " << priorityInfo.zoneid
//         //           << " | priority: " << priorityInfo.event_priority
//         //           << std::endl;

//         // If row not exists, perform UPDATE
//         char sql[MAX_SIZE];
//         char *zErrMsg = 0;
//         // if event status = disable -> set to enable
//         if (ret  == 0)
//         {
//             char *zErrMsg = 0;
//             memset(sql, 0, MAX_SIZE);

//             snprintf(sql, MAX_SIZE, UPDATE_DATA_EVENT_PRIORITY_PRE,
//                      priorityInfo.event_name.c_str(),
//                      priorityInfo.channelid,
//                      priorityInfo.zoneid.c_str());

//             ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
            
//             if (ret != SQLITE_OK)
//             {
//                 fprintf(stderr, "SQL event priority _pre update error: %s\n", zErrMsg);
//                 sqlite3_free(zErrMsg);
//                 return false;
//             }
//         }
//         // if event do not have in table -> insert event into table
//         else if( ret = 2)
//         {
//             char *zErrMsg = 0;
//             memset(sql, 0, MAX_SIZE);

//             snprintf(sql, MAX_SIZE, INSERT_DATA_EVENT_PRIORITY_PRE,
//                      priorityInfo.event_name.c_str(),
//                      priorityInfo.is_enable,
//                      priorityInfo.channelid,
//                      priorityInfo.zoneid.c_str(),
//                      priorityInfo.event_priority.c_str());

//             ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
//             if (ret != SQLITE_OK)
//             {
//                 fprintf(stderr, "SQL event priority _pre insert error: %s\n", zErrMsg);
//                 sqlite3_free(zErrMsg);
//                 return false;
//             }
//         }
//     }

//     return true;
// }

// bool DbSqlite::set_disable_event_priority(EventPriority &priorityInfo)
// {
//     if (db_ready)
//     {
//         // If row exists, perform UPDATE
//         char sql[MAX_SIZE];
//         char *zErrMsg = 0;
//             memset(sql, 0, MAX_SIZE);

//             snprintf(sql, MAX_SIZE, UPDATE_DATA_EVENT_PRIORITY_DISABLE,
//                      priorityInfo.event_name.c_str());

//             int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);

//             if (ret != SQLITE_OK)
//             {
//                 fprintf(stderr, "SQL event priority disable error: %s\n", zErrMsg);
//                 sqlite3_free(zErrMsg);
//                 return false;
//             }
//     }
//     return true;
// }

// bool DbSqlite::set_disable_all_event_priority()
// {
//     if (db_ready)
//     {
//         char *zErrMsg = 0;

//         int ret = sqlite3_exec(db, UPDATE_DATA_DISABLE_ALL_EVENT_PRIORITY, NULL, 0, &zErrMsg);

//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "SQL disable all event priority error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
//     }

//     return true;
// }

// bool DbSqlite::insert_event_priority(EventPriority &priorityInfo)
// {
//     if (db_ready)
//     {
//         // If row exists, perform UPDATE
//         char sql[MAX_SIZE];
//         char *zErrMsg = 0;
//         memset(sql, 0, MAX_SIZE);

//         snprintf(sql, MAX_SIZE, UPDATE_DATA_EVENT_PRIORITY,
//                  priorityInfo.event_priority.c_str(),
//                  priorityInfo.event_name.c_str(),
//                  priorityInfo.channelid,
//                  priorityInfo.zoneid.c_str());

//         int ret = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
        
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "SQL event priority update error: %s\n", zErrMsg);
//             sqlite3_free(zErrMsg);
//             return false;
//         }
      
//     }

//     return true;
// }

// bool DbSqlite::get_event_priority(std::vector<EventPriority> &eventPriorityVector)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         int ret = sqlite3_prepare_v2(db, SELECT_EVENT_PRIORITY_INFO, -1, &stmt, NULL);
//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from EVENT_PRIORITY error: %s\n");
//             sqlite3_free(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             EventPriority priorityInfo;

//             priorityInfo.id = sqlite3_column_int(stmt, 0);
//             priorityInfo.is_enable = sqlite3_column_int(stmt, 1);
//             priorityInfo.event_name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
//             priorityInfo.channelid = sqlite3_column_int(stmt, 3);            
//             priorityInfo.zoneid = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
            
//             const char* columnText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
//             if( columnText != nullptr)
//             {
//                 priorityInfo.event_priority = std::string(columnText);
//             }
//             // std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//             //           << " from db | id: " << priorityInfo.id
//             //           << " | event_name: " <<  priorityInfo.event_name
//             //           << " | channelId: " << priorityInfo.channelid
//             //           << " | zoneId: " << priorityInfo.zoneid
//             //           << " | priority: " << priorityInfo.event_priority
//             //           << std::endl;

//             eventPriorityVector.push_back(priorityInfo);
//         }
//         sqlite3_free(stmt);
//     }
//     return true;
// }

// bool DbSqlite::get_enable_event(std::vector<EventPriority> &eventPriorityVector)
// {
//     if (db_ready)
//     {
//         sqlite3_stmt *stmt;
//         // char sql[MAX_SIZE];
//         // memset(sql, 0, MAX_SIZE);
//         // sprintf(sql, SELECT_INFO_EVENT_PRIORITY_ENABLE,
//         //             priorityInfo.event_name.c_str(),
//         //             priorityInfo.channelid,
//         //             priorityInfo.is_enable,
//         //             priorityInfo.zoneid);
//         int ret = sqlite3_prepare_v2(db, SELECT_INFO_EVENT_PRIORITY_ENABLE, -1, &stmt, NULL);

//         if (ret != SQLITE_OK)
//         {
//             fprintf(stderr, "Get data from EVENT_PRIORITY enable error: %s\n");
//             sqlite3_free(stmt);
//             return false;
//         }

//         while (sqlite3_step(stmt) == SQLITE_ROW)
//         {
//             EventPriority priorityInfo;

//             priorityInfo.id = sqlite3_column_int(stmt, 0);
//             priorityInfo.is_enable = sqlite3_column_int(stmt, 1);
//             priorityInfo.event_name = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2)));
//             priorityInfo.channelid = sqlite3_column_int(stmt, 3);
//             const char* columnText_1 = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
//             if(columnText_1 != nullptr)
//             {
//                 priorityInfo.zoneid = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
//             }
//             priorityInfo.zoneid = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4)));
//             const char* columnText = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
//             if(columnText != nullptr)
//             {
//                 priorityInfo.event_priority = std::string(reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5)));
//             }
//             eventPriorityVector.push_back(priorityInfo);
//         }
//         sqlite3_free(stmt);
//     }
//     return true;
// }

// void DbSqlite::check_db_version()
// {
//     // Check version
//     std::string dbVer;
//     std::ifstream readVerFile(DB_VERSION_FILE);

//     while(getline(readVerFile, dbVer))
//     {
//         if(!dbVer.compare(DB_VER))
//         {
//             std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//                       << " Same version: " << dbVer << std::endl;
//             return;
//         }
//     }

//     // Delete old data
//     std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//               << " Delete old data and create ver file: " << DB_VER << std::endl;
//     system(DELETE_DB_DIR_FALCONEYEGATEWAY);
//     system(MAKE_DB_DIR_FALCONEYEGATEWAY);
//     change_db_dir_ownership();

//     // Create latest ver file
//     std::ofstream writeVerFile(DB_VERSION_FILE);
//     writeVerFile << DB_VER;
//     writeVerFile.close();

//     return;
// }

// void DbSqlite::change_db_dir_ownership()
// {
//     struct stat sb;

//     if(stat(DB_DIR, &sb) == 0)
//     {
//         std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//                   << " Change owner ship of db dir: " << DB_DIR << std::endl;
//         system(CHANGES_OWNERSHIP);
//     }
//     else
//     {
//         std::cout << __LINE__ << " [F LOG] " << __PRETTY_FUNCTION__
//                   << " Dir does not exist, do not change ownership: " << DB_DIR << std::endl;
//     }

//     return;
// }
