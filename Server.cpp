// Server.cpp
#include "Server.h"
#include "DbSqlite.h"

Server::Server() : isConnected(true),isAdmin(false)
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);
    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }
    if (listen(serverSocket, 10) == -1)
    {
        std::cerr << "Error listening on socket\n";
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port 8888...\n";
}

Server::~Server()
{
    close(serverSocket);
}

void Server::start()
{
    while (isConnected)
    {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (!isConnected)
        {
            // Nếu kết nối đã đóng, thoát khỏi vòng lặp
            break;
        }
        // Send confirmation to the client
        const char *confirmationMessage = "Connection established. Welcome to the server!";
        send(clientSocket, confirmationMessage, strlen(confirmationMessage), 0);

        std::cout << "Sent confirmation to client\n";

        // Handle other client requests here...

        // Receive client request type
        while (true)
        {
            char requestType[1024] = {0};
            memset(requestType, 0, 1024);

            recv(clientSocket, &requestType, sizeof(requestType) - 1, 0);
            std::string str_req(requestType);
            std::cout << str_req << "\n";

            // Handle the request based on the type
            if (str_req == LOGIN)
            {
                handleLogin(clientSocket);
            }
            else if (str_req == REGISTER)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == CREATE_ROOM)
            {
                handleCreateExamRoom(clientSocket);
            }
            else if (str_req == UPDATE_DURATION)
            {
                handleSetExamDuration(clientSocket);
            }
            else if (str_req == VIEW_STATUS_ROOM)
            {
                handleViewStatusRoom(clientSocket);
            }
            else if (str_req == JOIN_ROOM)
            {
                handleJoinRoom(clientSocket);
            }
            else if (str_req == VIEW_STATUS_ROOM)
            {
                handleViewStatusRoom(clientSocket);
            }
            else if (str_req == VIEW_RUSULT_ROOM)
            {
                handleViewRusultRoom(clientSocket);
            }
            else if (str_req == START_EXAM)
            {
                // handleStartExam(clientSocket);
            }
            else if (str_req == UPDATE_TIME_END_ROOM)
            {
                // handleUpdateEndTime(clientSocket);
            }
            else if (str_req == TRAINING_MODE)
            {
                handleTrainningMode(clientSocket);
            }
            else if (str_req == LOGOUT)
            {
                handleLogout(clientSocket);
            }
            // else if (str_req == NUMBER_QUESTION_TRAINING_MODE)
            // {
            //     /* code */
            // }
        }
        close(clientSocket);
    }
}

void Server::handleLogin(int clientSocket)
{
    std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Receive username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    // Extract username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    User user;
    std::string username, password;
    username = credentials.substr(0, pos);
    password = credentials.substr(pos + 1);
    strncpy(user.username, username.c_str(), sizeof(user.username));
    std::cout << "| user.username: " << user.username << "\n";
    strncpy(user.password, password.c_str(), sizeof(user.password));
    std::cout << "| user.password: " << user.password << "\n";
    if (!(strcmp(user.username, "admin") && strcmp(user.password, "admin")))
    {
        isAdmin = true;
        const char *successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else if (DbSqlite::getInstance()->search_id_user(user) != -1)
    {
        const char *successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else
    {
        const char *errorMessage = "NotOk";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

void Server::handleRegistration(int clientSocket)
{
    std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive new username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "|tin nhan tu regis server: " << buffer << std::endl;

    // Extract new username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    User user;
    std::string username, password;
    username = credentials.substr(0, pos);
    password = credentials.substr(pos + 1);
    strncpy(user.username, username.c_str(), sizeof(user.username));
    strncpy(user.password, password.c_str(), sizeof(user.password));
    std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";
    // DbSqlite::getInstance()->insert_user_data(user);
    std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";
    if (DbSqlite::getInstance()->insert_user_data(user))
    {
        std::cout << "Regis oke";
        const char *successMessage = "Registration successful";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else
    {
        std::cout << "Regis not oke";
        const char *errorMessage = "Registration failed";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

void Server::handleCreateExamRoom(int clientSocket)
{
    std::cout << "|Insight Create Room\n";
    Room roominfo;
    std::cout << __LINE__ << std::endl;
    // int timeDuration;
    if (recv(clientSocket, &roominfo, sizeof(roominfo), 0) == -1)
    {
        std::cerr << "Error recieving data to the server\n";
        close(clientSocket);
        close(serverSocket);
    }
    // else
    // {
    //     // close(clientSocket);
    // }
    // close(serverSocket);

    std::cout << roominfo.name << " " << roominfo.numberQuestion << " "
              << roominfo.numberQuestion << "\n";
    // std::cout << timeDuration << std::endl;

    DbSqlite::getInstance()->insert_room_data(roominfo);
}

void Server::handleSetNumberOfQuestions(int clientSocket)
{
    const char *responseMessage = "Number of questions set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void Server::handleSetExamDuration(int clientSocket)
{
    std::cout << "|insight handle set exem duration\n";
    std::vector<Room> roomInfo;
    std::cout << __LINE__ << std::endl;
    DbSqlite::getInstance()->get_room_info(roomInfo);
    int size = roomInfo.size();
    send(clientSocket, &size, sizeof(size), 0);
    send(clientSocket, &roomInfo[0], roomInfo.size() * sizeof(roomInfo), 0);

    Room room;
    recv(clientSocket, &room, sizeof(room), 0);
    DbSqlite::getInstance()->update_timeDuration(room);
    std::cout << "Handling set exam duration request\n";
    // insert number durration
    // const char *responseMessage = "Exam duration set successfully";
    // send(clientSocket, responseMessage, strlen(responseMessage), 0);
}
void Server::handleTrainningMode(int clientSocket)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    std::vector<Question> question;
    DbSqlite::getInstance()->get_question_info(question);
    int size_vec = question.size();
    send(clientSocket, &size_vec, sizeof(size_vec), 0);
    for (size_t i = 0; i < question.size(); i++)
    {
        Question ques = question[i];
        send(clientSocket, &ques, sizeof(ques), 0);
    }
    std::cout << "Send done quesiton for training mode\n";

    // DbSqlite::getInstance()->get();
}
void Server::handleJoinRoom(int clientSocket)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
    std::vector<Room> roomInfo;
    std::vector<Room> alreadyRoomInfo;

    DbSqlite::getInstance()->get_room_info(roomInfo);
    std::cout << __LINE__ << " : " << __FUNCTION__ << "roomInfo: " << roomInfo.size() << std::endl;

    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        // std::cout << "|roominfo. name: " << roomInfo[i].name
        // << "; " << "|roominfo.status: " << roomInfo[i].status << std::endl;
        if (roomInfo[i].status == 0)
        {
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
            std::cout << "|room available: " << roomInfo[i].name << " " << std::endl;
            std::cout << "|timeduration: " << roomInfo[i].timeDuration << " " << std::endl;
            std::cout << "|numberofquestion: " << roomInfo[i].numberQuestion << " " << std::endl;
            alreadyRoomInfo.push_back(roomInfo[i]);
        }
    }
    int _size = alreadyRoomInfo.size();
    send(clientSocket, &_size, sizeof(_size), 0);

    for (const Room &room : alreadyRoomInfo)
    {
        send(clientSocket, &room, sizeof(Room), 0);
    }
    // send(clientSocket, &alreadyRoomInfo[0], alreadyRoomInfo.size() * sizeof(alreadyRoomInfo), 0);

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string strNameRoom(buffer);
    std::cout << "|strNameRoom: " << strNameRoom << std::endl;
    if (strNameRoom == RETURN_MAIN_MENU)
    {
        return;
    }

    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string strNameUser(buffer);

    DbSqlite::getInstance()->update_user_room(strNameUser, strNameRoom);

    Room room;
    strncpy(room.name, strNameRoom.c_str(), sizeof(room.name));
    strncpy(room.user, strNameUser.c_str(), sizeof(room.user));
    // room.name = strNameRoom.c_str();
    // room.user = strNameUser.c_str();
    int reqRoom;
    recv(clientSocket, &reqRoom, sizeof(reqRoom), 0);
    if (reqRoom == 1)
    {
        handleStartExam(clientSocket, room);
    }
    else
    {
        return;
    }
}
void Server::handleStartExam(int clientSocket, Room room)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    std::vector<Question> question;
    DbSqlite::getInstance()->get_question_info(question);
    int _size = question.size();
    std::cout << __LINE__ << " : " << __FUNCTION__ << "|question.size(): "
              << question.size() << std::endl;

    send(clientSocket, &_size, sizeof(_size), 0);
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    for (size_t i = 0; i < question.size(); i++)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Question ques = question[i];
        send(clientSocket, &ques, sizeof(ques), 0);
    }

    // for (const Question& ques : question) {
    //     send(clientSocket, &ques, sizeof(question), 0);
    // }
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    // send(clientSocket, &question[0], question.size() * sizeof(question), 0);
    // int timeDuration;
    // DbSqlite::getInstance()->get_timeDuration(room.name, timeDuration);
    // std::cout << "|timeDuration: " << timeDuration << std::endl;
    // send(clientSocket, &timeDuration, sizeof(timeDuration), 0);
}
void Server::handleViewRusultRoom(int clientSocket)
{
      std::vector<Room> roomInfo;
    DbSqlite::getInstance()->get_room_info(roomInfo);
    int _size = roomInfo.size();
    send(clientSocket, &_size, sizeof(_size), 0);

    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        Room room = roomInfo[i];
        send(clientSocket, &room, sizeof(room), 0);
    }
    
}

void Server::handleViewStatusRoom(int clientSocket)
{
    std::vector<Room> roomInfo;
    DbSqlite::getInstance()->get_room_info(roomInfo);
    int _size = roomInfo.size();
    send(clientSocket, &_size, sizeof(_size), 0);

    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        Room room = roomInfo[i];
        send(clientSocket, &room, sizeof(room), 0);
    }
    
}

void Server::handleLogout(int clientSocket)
{}