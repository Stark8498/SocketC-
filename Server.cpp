// Server.cpp
#include "Server.h"
#include "DbSqlite.h"

/**
 * The Server constructor creates a socket, binds it to a specific address and port, and starts
 * listening for incoming connections.
 */
Server::Server() : isConnected(true), isAdmin(false)
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
    // close(serverSocket);
}

void Server::start()
{
    std::cout << "[LINE] " << __LINE__
              << " FUNC: " << __FUNCTION__ << "\n";
    while (isConnected)
    {
        std::cout << "[LINE] " << __LINE__
                  << " FUNC: " << __FUNCTION__ << "\n";
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
            else
            {
                break;
            }
            // else if (str_req == NUMBER_QUESTION_TRAINING_MODE)
            // {
            //     /* code */
            // }
        }
        close(clientSocket);
    }
}

/**
 * The function `handleLogin` receives a username and password from a client, checks if the credentials
 * are valid, and sends a success or error message back to the client.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor for the client connection.
 * It is used to send and receive data between the server and the client.
 */
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
    if (!(strcmp(user.username, "admin")) && !(strcmp(user.password, "admin")))
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
        isAdmin = true;
        const char *successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else if (DbSqlite::getInstance()->search_id_user(user) != -1)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        const char *successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        const char *errorMessage = "NotOk";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

/**
 * The function `handleRegistration` receives a new username and password from a client, checks if the
 * username already exists in the database, and either registers the user or sends an error message
 * back to the client.
 *
 * @param clientSocket The parameter `clientSocket` is the socket descriptor for the client connection.
 * It is used to send and receive data between the server and the client.
 *
 * @return In this code snippet, the function does not have a return type specified. Therefore, it does
 * not explicitly return any value.
 */
void Server::handleRegistration(int clientSocket)
{
    std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive new username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    // std::cout << "|tin nhan tu regis server: " << buffer << std::endl;

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
    std::vector<User> userinfo;
    DbSqlite::getInstance()->get_user_info(userinfo);
    for (size_t i = 0; i < userinfo.size(); i++)
    {
        if (username == userinfo[i].username)
        {
            std::cout << "Regis oke";
            const char *successMessage = "User name already exists ";
            send(clientSocket, successMessage, strlen(successMessage), 0);
            return;
        }
    }

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

/**
 * The function `handleCreateExamRoom` receives data from a client socket, inserts room information
 * into a database, and inserts a set of questions into the database based on the room information.
 *
 * @param clientSocket The `clientSocket` parameter is the socket descriptor for the client connection.
 * It is used to receive data from the client and send responses back to the client.
 */
void Server::handleCreateExamRoom(int clientSocket)
{
    std::cout << "|Insight Create Room\n";
    Total_room roominfo;
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

    // std::cout << roominfo.name << " " << roominfo.numberQuestion << " "
    //           << roominfo.easy << " " << roominfo.topic << "\n";
    // std::cout << timeDuration << std::endl;
    int numberQuestion = roominfo.easy + roominfo.normal + roominfo.difficult + roominfo.veryhard;
    DbSqlite::getInstance()->insert_room_data(roominfo);
    Question questions;
    for (int i = 1; i < 100; ++i)
    {
        // std::string question = "Question " + std::to_string(i);
        // strncpy(questions.content, question.c_str(), sizeof(questions.content));
        // std::string answerA = "Option A for Question " + std::to_string(i);
        // strncpy(questions.choices1, answerA.c_str(), sizeof(questions.choices1));

        // std::string answerB = "Option B for Question " + std::to_string(i);
        // strncpy(questions.choices2, answerB.c_str(), sizeof(questions.choices2));

        // std::string answerC = "Option C for Question " + std::to_string(i);
        // strncpy(questions.choices3, answerC.c_str(), sizeof(questions.choices3));

        // std::string answerD = "Option D for Question " + std::to_string(i);
        // strncpy(questions.choices4, answerD.c_str(), sizeof(questions.choices4));
        std::string correctAnswer;
        std::string topic;
        topic = roominfo.topic;
        int level;
        if (i < 20)
        {
            std::string str = "easy";
            std::string question = "Question " + str + std::to_string(i);
            strncpy(questions.content, question.c_str(), sizeof(questions.content));
            std::string answerA = "Option A for Question " + std::to_string(i);
            strncpy(questions.choices1, answerA.c_str(), sizeof(questions.choices1));

            std::string answerB = "Option B for Question " + std::to_string(i);
            strncpy(questions.choices2, answerB.c_str(), sizeof(questions.choices2));

            std::string answerC = "Option C for Question " + std::to_string(i);
            strncpy(questions.choices3, answerC.c_str(), sizeof(questions.choices3));

            std::string answerD = "Option D for Question " + std::to_string(i);
            strncpy(questions.choices4, answerD.c_str(), sizeof(questions.choices4));
            correctAnswer = "A";
            level = 0;
        }
        else if (i > 20 && i < 40)
        {
            std::string str = "normal";
            std::string question = "Question " + str + std::to_string(i);
            strncpy(questions.content, question.c_str(), sizeof(questions.content));
            std::string answerA = "Option A for Question " + std::to_string(i);
            strncpy(questions.choices1, answerA.c_str(), sizeof(questions.choices1));

            std::string answerB = "Option B for Question " + std::to_string(i);
            strncpy(questions.choices2, answerB.c_str(), sizeof(questions.choices2));

            std::string answerC = "Option C for Question " + std::to_string(i);
            strncpy(questions.choices3, answerC.c_str(), sizeof(questions.choices3));

            std::string answerD = "Option D for Question " + std::to_string(i);
            strncpy(questions.choices4, answerD.c_str(), sizeof(questions.choices4));
            correctAnswer = "A";
            level = 1;
        }
        else if (i > 40 && i < 60)
        {
            std::string str = "difficult";
            std::string question = "Question " + str + std::to_string(i);
            strncpy(questions.content, question.c_str(), sizeof(questions.content));
            std::string answerA = "Option A for Question " + std::to_string(i);
            strncpy(questions.choices1, answerA.c_str(), sizeof(questions.choices1));

            std::string answerB = "Option B for Question " + std::to_string(i);
            strncpy(questions.choices2, answerB.c_str(), sizeof(questions.choices2));

            std::string answerC = "Option C for Question " + std::to_string(i);
            strncpy(questions.choices3, answerC.c_str(), sizeof(questions.choices3));

            std::string answerD = "Option D for Question " + std::to_string(i);
            strncpy(questions.choices4, answerD.c_str(), sizeof(questions.choices4));
            correctAnswer = "C";
            level = 2;
        }
        else
        {
            std::string str = "veryhard";
            std::string question = "Question " + str + std::to_string(i);
            strncpy(questions.content, question.c_str(), sizeof(questions.content));
            std::string answerA = "Option A for Question " + std::to_string(i);
            strncpy(questions.choices1, answerA.c_str(), sizeof(questions.choices1));

            std::string answerB = "Option B for Question " + std::to_string(i);
            strncpy(questions.choices2, answerB.c_str(), sizeof(questions.choices2));

            std::string answerC = "Option C for Question " + std::to_string(i);
            strncpy(questions.choices3, answerC.c_str(), sizeof(questions.choices3));

            std::string answerD = "Option D for Question " + std::to_string(i);
            strncpy(questions.choices4, answerD.c_str(), sizeof(questions.choices4));
            correctAnswer = "B";
            level = 3;
        }

        strncpy(questions.correctAnswer, correctAnswer.c_str(), sizeof(questions.correctAnswer));
        strncpy(questions.topic, topic.c_str(), sizeof(questions.topic));
        questions.level = level;

        // std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";
        DbSqlite::getInstance()->insert_question_data(questions);
    }
}

void Server::handleSetNumberOfQuestions(int clientSocket)
{
    const char *responseMessage = "Number of questions set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

/**
 * The function `handleSetExamDuration` handles the request to set the exam duration by sending the
 * room information to the client and updating the time duration in the database.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor for the client connection.
 * It is used to send and receive data between the server and the client.
 *
 * @return In this code snippet, nothing is being explicitly returned. The function has a void return
 * type, which means it does not return any value.
 */
void Server::handleSetExamDuration(int clientSocket)
{
    std::cout << "|insight handle set exem duration\n";
    std::vector<Total_room> roomInfo;
    std::cout << __LINE__ << std::endl;
    DbSqlite::getInstance()->get_room_info(roomInfo);
    int size = roomInfo.size();
    send(clientSocket, &size, sizeof(size), 0);
    for (int i = 0; i < roomInfo.size(); i++)
    {
        Total_room room_p = roomInfo[i];
        send(clientSocket, &room_p, sizeof(room_p), 0);
    }

    Total_room room;
    char name[1024];
    recv(clientSocket, &name, sizeof(name), 0);
    std::string nameStr(name);
    if (nameStr == RETURN_MAIN_MENU)
    {
        return;
    }

    int timeDur;
    recv(clientSocket, &timeDur, sizeof(timeDur), 0);
    strcpy(room.name, name);
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
/**
 * The function `handleJoinRoom` handles the process of a client joining a room in a server
 * application.
 *
 * @param clientSocket The parameter `clientSocket` is the socket descriptor for the client connection.
 * It is used to send and receive data between the server and the client.
 *
 * @return The function does not have a return type specified, so it does not explicitly return
 * anything.
 */
void Server::handleJoinRoom(int clientSocket)
{
    std::vector<Total_room> roomInfo;
    std::vector<Total_room> alreadyRoomInfo;

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
            alreadyRoomInfo.push_back(roomInfo[i]);
        }
    }
    int _size = alreadyRoomInfo.size();
    send(clientSocket, &_size, sizeof(_size), 0);

    for (const Total_room &room : alreadyRoomInfo)
    {
        send(clientSocket, &room, sizeof(room), 0);
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
    int id_room = DbSqlite::getInstance()->get_id_total_room(strNameRoom);
    int reqRoom;
    recv(clientSocket, &reqRoom, sizeof(reqRoom), 0);
    if (reqRoom == 1)
    {
        handleStartExam(clientSocket, id_room, strNameUser);
    }
    else
    {
        return;
    }
}
/**
 * The function `handleStartExam` in the `Server` class sends a list of questions to a client, receives
 * the client's score, and inserts the score into a database.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor for the client connection.
 * It is used to send and receive data between the server and the client.
 * @param id_room The `id_room` parameter represents the ID of the room where the exam is taking place.
 * @param user The "user" parameter in the "handleStartExam" function is a string that represents the
 * username of the client who initiated the exam.
 */
void Server::handleStartExam(int clientSocket, int id_room, std::string user)
{
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

    std::vector<Question> question;
    DbSqlite::getInstance()->get_question_info(question);
    int _size = question.size();

    std::cout << __LINE__ << " : " << __FUNCTION__ << "|question.size(): "
              << question.size() << std::endl;

    send(clientSocket, &_size, sizeof(_size), 0);
    int _size_p = 10;
    send(clientSocket, &_size_p, sizeof(_size_p), 0);
    std::cout << __LINE__ << " : " << __FUNCTION__ << _size << std::endl;

    for (size_t i = 0; i < question.size(); i++)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Question ques = question[i];
        send(clientSocket, &ques, sizeof(ques), 0);
    }
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
    int score;
    recv(clientSocket, &score, sizeof(score), 0);
    DbSqlite::getInstance()->insert_userscore_inforoom(id_room, user, score);
}
/**
 * The function `handleViewResultRoom` retrieves information about rooms from a database and sends it
 * to a client socket.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor of the client that is
 * connected to the server. It is used to send data back to the client.
 */
/**
 * The function `handleViewResultRoom` retrieves information about rooms from a database and sends it
 * to a client socket.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor of the client that is
 * connected to the server. It is used to send data back to the client.
 */
void Server::handleViewRusultRoom(int clientSocket)
{
    std::vector<Room_info> roomInfo;
    std::vector<Total_room> vecTotalRoom;
    DbSqlite::getInstance()->get_room_info(vecTotalRoom);

    for (int i = 0; i < vecTotalRoom.size(); i++)
    {
        std::cout << "|id: " << vecTotalRoom[i].id;
        std::cout << "\n|name: " << vecTotalRoom[i].name
                  << std::endl;
    }

    DbSqlite::getInstance()->get_info_inforoom(roomInfo);
    int _size = roomInfo.size();
    std::cout << __LINE__ << " : " << __FUNCTION__ << "|Size: " << _size << std::endl;

    send(clientSocket, &_size, sizeof(_size), 0);
    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << "|roomInfo[i].id_totalroom: " << roomInfo[i].id_totalroom << std::endl;

        Room_result result;
        for (int j = 0; j < vecTotalRoom.size(); j++)
        {
            if (vecTotalRoom[j].id == roomInfo[i].id_totalroom)
            {
                std::string nameStr(vecTotalRoom[j].name);
                strcpy(result.name, nameStr.c_str());
                strcpy(result.user, roomInfo[i].user);
                result.score = roomInfo[i].score;
                send(clientSocket, &result, sizeof(result), 0);
            }
        }

        // std::string nameStr = DbSqlite::getInstance()->get_namemroom_from_id(roomInfo[i].id_totalroom);
    }
}

/**
 * The function `handleViewStatusRoom` in the `Server` class sends the information of all rooms to a
 * client socket.
 *
 * @param clientSocket The clientSocket parameter is the socket descriptor for the client connection.
 * It is used to send data back to the client.
 */
void Server::handleViewStatusRoom(int clientSocket)
{
    std::vector<Total_room> roomInfo;
    DbSqlite::getInstance()->get_room_info(roomInfo);
    int _size = roomInfo.size();
    send(clientSocket, &_size, sizeof(_size), 0);

    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        Total_room room = roomInfo[i];
        send(clientSocket, &room, sizeof(room), 0);
    }
}

void Server::handleLogout(int clientSocket)
{
}

// void Server::handleCreateNewRoom(int clientSocket)
// {
//     char newRoom[100];
//     memset(newRoom, 0, sizeof(newRoom));
//     recv(clientSocket, &newRoom, sizeof(newRoom), 0);
//     DbSqlite::getInstance()->creat_room_info(newRoom);

//     Room_info roominfo;
//     std::cout << __LINE__ << std::endl;
//     // int timeDuration;
//     if (recv(clientSocket, &roominfo, sizeof(roominfo), 0) == -1)
//     {
//         std::cerr << "Error recieving data to the server\n";
//         close(clientSocket);
//         close(serverSocket);
//     }
//     // else
//     // {
//     //     // close(clientSocket);
//     // }
//     // close(serverSocket);

//     std::cout << roominfo.numberQuestion << " "
//               << roominfo.easy << " " << roominfo.topic << "\n";
//     // std::cout << timeDuration << std::endl;

//     DbSqlite::getInstance()->insert_room_info(newRoom, roominfo);
// }