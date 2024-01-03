#include "Client.h"

// function
std::vector<std::string> splitString(const std::string &inputString, char delimiter)
{
    std::istringstream iss(inputString);
    std::vector<std::string> tokens;

    std::string token;
    while (std::getline(iss, token, delimiter))
    {
        int start = token.find_first_not_of(" ");
        int end = token.find_last_not_of(" ");
        if (start != std::string::npos && end != std::string::npos)
        {
            tokens.push_back(token.substr(start, end - start + 1));
        }
    }

    return tokens;
}

void Client::showTest(std::vector<Question> question, int timeDuration, int numberQuestion, int &correctAnswers, bool isTraining)
{
    srand(time(0));

    // Question questions[5];
    // std::cout << "Start Question Test"
    //           << "|numberQuestion: " << numberQuestion << "|timeDuration: " << timeDuration << std::endl;
    correctAnswers = 0;
    time_t startTime = time(0);
    time_t elapsedTime;
    int i = 0;
    if (numberQuestion > question.size())
    {
        numberQuestion = question.size();
    }
    if (numberQuestion == 0)
    {
        std::cout << "There are no matching questions in the DB \n";
        outfile << "There are no matching questions in the DB \n";

        return;
    }
    while (true)
    {
        // if (i > numberQuestion)
        // {
        //     break;
        // }

        // // std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

        elapsedTime = time(0) - startTime;
        if (elapsedTime >= timeDuration)
        {
            std::cout << "End Time ! END TEST" << std::endl;
            return;
        }
        if (i < numberQuestion)
        {
            std::string userAnswer = askQuestion(question[i]);

            if (strcmp(question[i].correctAnswer, userAnswer.c_str()) == 0)
            {
                // std::cout << "Right!" << std::endl;
                correctAnswers++;
            }
            std::cout << "Time remain: " << (timeDuration - elapsedTime) << std::endl;
            // std::cout << "[LINE]" << __LINE__ << "[FUNC]" << __FUNCTION__ << "|correctAnswers: " << correctAnswers << std::endl;

            if (elapsedTime >= timeDuration)
            {
                // std::cout << "[LINE]" << __LINE__ << "[FUNC]" << __FUNCTION__ << "|correctAnswers: " << correctAnswers << std::endl;

                std::cout << "End Time ! END TEST" << std::endl;
                return;
            }
        }

        // sleep(2);
        if (elapsedTime >= timeDuration)
        {
            std::cout << "End Time ! END TEST" << std::endl;
            return;
        }
        if (i >= numberQuestion)
        {
            std::cout << "What question do you still have time to change your answer to? " << std::endl;
            if (elapsedTime >= timeDuration)
            {
                std::cout << "End Time ! END TEST" << std::endl;
                return;
            }
            std::cout << "Time remain: " << (timeDuration - elapsedTime) << std::endl;

            std::string choice_change;
            while (true)
            {
                // std::regex pattern("[1-2]");

                std::cout << "Are you change your answer exam ? " << std::endl;
                std::cout << "choice 1 for Yes\n";
                std::cout << "choice 2 for No, Summit exam\n";
                std::cin >> choice_change;
                std::regex pattern("[1-2]");
                if (std::regex_match(choice_change, pattern))
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid choice, Enter choice again !" << std::endl;
                }
            }
            int choice_change_int = std::stoi(choice_change);

            if (choice_change_int == 1)
            {
                std::string input;
                while (true)
                {
                    std::cout << "Number question change: ";
                    std::cin >> input;
                    std::regex pattern("^[0-9]+$");
                    if (std::regex_match(input, pattern))
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Invalid choice, Enter choice again !" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                int number = std::stoi(input);
                std::string answ;
                while (true)
                {
                    std::cout << "Choice answer (A-B): ";
                    std::cin >> answ;
                    std::regex pattern("[ABCD]");
                    if (std::regex_match(answ, pattern))
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Invalid choice, Enter choice again(A-D) !" << std::endl;
                    }
                }

                if (strcmp(question[number].correctAnswer, answ.c_str()) == 0)
                {
                    correctAnswers++;
                }
                // break;
            }
            else if (choice_change_int == 2)
            {
                break;
            }
        }
        i++;
        // std::cout << "[LINE]" << __LINE__ << "[FUNC]" << __FUNCTION__ << "|correctAnswers: " << correctAnswers << std::endl;
    }
}

std::string Client::askQuestion(Question q)
{
    std::string userAnswer;
    while (true)
    {
        std::cout << q.content << std::endl;
        std::cout << q.choices1 << std::endl;
        std::cout << q.choices2 << std::endl;
        std::cout << q.choices3 << std::endl;
        std::cout << q.choices4 << std::endl;
        std::cin >> userAnswer;
        std::cout << "Choose answer (A-B): ";
        std::regex pattern("[ABCD]");
        if (std::regex_match(userAnswer, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, PLEASE ENTER A-D!" << std::endl;
        }
    }

    return userAnswer;
}

Client::Client() : isConnected(false), outfile("LOG.txt", std::ios::trunc), isLogin(false), isAdmin(false)
{
    // std::ofstream outfile("LOG.txt");
    outfile << "---Hello, init Client, start log----\n";

    //
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        outfile << "__Contructor__ "
                << "Error creating socket\n";

        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
}

Client::~Client()
{
    disconnect();
}

void Client::showSubMenu()
{
    std::string choice_str;
    while (true)
    {
        std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        switch (isAdmin)
        {
        case true:
        {
            std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

            while (true)
            {
                std::cout << "\n===== Menu =====\n";
                std::cout << "1. Create New Exam Room\n";
                std::cout << "2. Update Durationn\n";
                std::cout << "3. View Trace Log System\n";
                std::cout << "4. Join Room\n";
                std::cout << "5. View Status Room\n";
                std::cout << "6. View Result Room\n";
                std::cout << "7. Training Mode\n";
                std::cout << "8. Logout\n";

                std::cout << "Enter your choice: ";
                std::cin >> choice_str;
                std::regex pattern("[1-8]");
                if (std::regex_match(choice_str, pattern))
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid choice, Enter choice again !" << std::endl;
                }
            }
            int choice = std::stoi(choice_str);
            std::cout << "[LINE] " << __LINE__
                      << " FUNC: " << __FUNCTION__
                      << " choice: " << choice << "\n";
            std::string str_choice;
            if (choice == 1)
            {
                str_choice = CREATE_ROOM;
            }
            else if (choice == 2)
            {
                str_choice = UPDATE_DURATION;
            }
            else if (choice == 3)
            {
                // str_choice = UPDATE_TIME_END_ROOM;
            }
            else if (choice == 4)
            {
                str_choice = JOIN_ROOM;
            }
            else if (choice == 5)
            {
                str_choice = VIEW_STATUS_ROOM;
            }
            else if (choice == 6)
            {
                str_choice = VIEW_RUSULT_ROOM;
            }
            else if (choice == 7)
            {
                str_choice = TRAINING_MODE;
            }
            else if (choice == 8)
            {
                str_choice = LOGOUT;
            }
            // std::cout << "|str_choice: " << str_choice << "\n";
            // outfile << "|str_choice: " << str_choice << "\n";
            std::cout << "[LINE] " << __LINE__
                      << " FUNC: " << __FUNCTION__
                      << " str_choice: " << str_choice << "\n";
            if (str_choice.length() != 0)
            {
                send(clientSocket, str_choice.c_str(), strlen(str_choice.c_str()), 0);
            }
            switch (choice)
            {

            case 1:
            {
                createExamRoom();
                break;
            }
            case 2:
            {
                setExamDuration();
                break;
            }
            case 3:
            {
                viewLogTrace();
                break;
            }
            case 4:
            {
                joinRoom();
                break;
            }
            case 5:
                viewStatusRoom();
                break;
            case 6:
                resultRoom();
                break;
            case 7:
                trainingMode();
                break;
            case 8:
                Logout();
                break;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
            }

            break;
        }
        case false:
        {
            while (true)
            {
                std::cout << "\n===== Menu =====\n";
                std::cout << "1. Join_rom\n";
                std::cout << "2. Training_mode\n";
                std::cout << "3. Exit\n";

                std::cout << "Enter your choice: ";
                std::cin >> choice_str;
                std::regex pattern("[1-3]");
                if (std::regex_match(choice_str, pattern))
                {
                    break;
                }
                else
                {
                    std::cout << "Invalid choice, Enter choice again !" << std::endl;
                }
            }
            int choice = std::stoi(choice_str);
            std::string str_choice;
            if (choice == 1)
            {
                str_choice = JOIN_ROOM;
            }
            else if (choice == 2)
            {
                str_choice = TRAINING_MODE;
            }
            // std::cout << "|str_choice: " << str_choice << "\n";
            // outfile << "|str_choice: " << str_choice << "\n";

            if (str_choice.length() != 0)
            {
                send(clientSocket, str_choice.c_str(), strlen(str_choice.c_str()), 0);
            }
            switch (choice)
            {

            case 1:
                joinRoom();
                break;
            case 2:
            {
                trainingMode();
                break;
            }
            case 3:
                return;
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
            }

            break;
        }
        }
    }
}
void Client::createExamRoom()
{
    // std::cout << "|Insight create Room";
    Total_room roominfo;
    std::cout << "Enter new room name: ";
    std::cin >> roominfo.name;

    std::string timerDurationStr;
    while (true)
    {
        std::cout << "Enter time duration: ";
        // std::cin >> roominfo.timeDuration;
        std::cin >> timerDurationStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(timerDurationStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    roominfo.timeDuration = std::stoi(timerDurationStr);
    std::cout << "Enter Topic: ";
    std::cin >> roominfo.topic;

    std::string easeStr;
    while (true)
    {
        std::cout << "Enter new number of question  for level easy : ";
        // std::cin >> roominfo.timeDuration;
        std::cin >> easeStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(easeStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    roominfo.easy = std::stoi(easeStr);
    std::string normalStr;
    while (true)
    {
        std::cout << "Enter new number of question  for level normal : ";
        // std::cin >> roominfo.timeDuration;
        std::cin >> normalStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(normalStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    roominfo.normal = std::stoi(normalStr);
    std::string difficultStr;
    while (true)
    {
        std::cout << "Enter new number of question  for level difficult : ";
        // std::cin >> roominfo.timeDuration;
        std::cin >> difficultStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(difficultStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    roominfo.difficult = std::stoi(difficultStr);
    std::string veryhardStr;
    while (true)
    {
        std::cout << "Enter new number of question  for level veryhard : ";
        // std::cin >> roominfo.timeDuration;
        std::cin >> veryhardStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(veryhardStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    roominfo.veryhard = std::stoi(veryhardStr);
    roominfo.status = 0;
    if (send(clientSocket, &roominfo, sizeof(roominfo), 0) == -1)
    {
        std::cerr << "Error sending data to the server.\n";
        close(clientSocket);
        return;
    }
    std::cout << "Create New Room OK";
    outfile << "Create New Room OK";
}

void Client::setNumberOfQuestions()
{
    std::string numberofQuestionStr;
    while (true)
    {
        std::cout << "Enter numberOfQuestion: ";
        std::cin >> numberofQuestionStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(numberofQuestionStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    int numberofQuestion = std::stoi(numberofQuestionStr);
    send(clientSocket, &numberofQuestion, sizeof(numberofQuestion), 0);
    std::cout << "Set timeduration OK";
    outfile << "Set timeduration OK";
}

void Client::setExamDuration()
{
    // std::cout
    std::cout << "Please choose room: ";
    std::vector<Total_room> roomInfo;
    int _size;
    recv(clientSocket, &_size, sizeof(_size), 0);
    std::cout << __LINE__ << "|_size: " << _size << std::endl;
    for (int i = 0; i < _size; i++)
    {
        Total_room room;
        recv(clientSocket, &room, sizeof(room), 0);
        roomInfo.push_back(room);
        std::cout << "|Name of room: ";
        std::string strName(roomInfo[i].name);
        std::cout << "|Name Room: " << strName;
        std::cout << "\n|Time Duratition Room: " << roomInfo[i].timeDuration;

        std::cout << "\n====================\n";
    }
    std::string nameOfRoom;

    bool ret = false;
    std::cout << "\nChoose room: ";
    while (true)
    {
        if (ret)
        {
            break;
        }
        else
        {
            std::cin >> nameOfRoom;
            for (int i = 0; i < roomInfo.size(); i++)
            {
                if (strcmp(roomInfo[i].name, nameOfRoom.c_str()) == 0)
                {
                    ret = true;
                    break;
                }
            }
            std::cout << "No Matched Room, Please enter again\n";
        }
    }

    int count = 0;
    for (int i = 0; i < _size; i++)
    {
        if ((strcmp(roomInfo[i].name, nameOfRoom.c_str()) == 0))
        {
            send(clientSocket, nameOfRoom.c_str(), strlen(nameOfRoom.c_str()), 0);
            count++;
            break;
        }
    }
    if (!count)
    {
        std::cout << "No name room match ";
        std::string req_return = RETURN_MAIN_MENU;
        send(clientSocket, req_return.c_str(), strlen(req_return.c_str()), 0);
        return;
    }

    // send(clientSocket, room.name, sizeof(room.name), 0);

    std::string timerDurStr;
    while (true)
    {
        std::cout << "Enter numberOfQuestion: ";
        std::cin >> timerDurStr;
        std::regex pattern("^[0-9]+$");
        if (std::regex_match(timerDurStr, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    int timeDur = std::stoi(timerDurStr);
    send(clientSocket, &timeDur, sizeof(timeDur), 0);

    std::cout << "Set timeduration OK";
    outfile << "Set timeduration OK";
}

void Client::connectToServer()
{
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        outfile << "Error connecting to server\n";

        exit(EXIT_FAILURE);
    }

    isConnected = true;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive confirmation from the server
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Server response: " << buffer << std::endl;
    outfile << "Server response: " << buffer << std::endl;
    while (true)
    {
        showMenu();
    }
}

void Client::disconnect()
{
    if (isConnected)
    {
        close(clientSocket);
        isConnected = false;
        std::cout << "Disconnected from server\n";
        outfile << "Disconnected from server\n";
    }
    else
    {
        std::cout << "Exit programe\n";
        outfile << "Exit programe\n";
    }
}

void Client::showMenu()
{
    std::string choice_str;
    std::string str_choice;
    while (true)
    {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Log in\n";
        std::cout << "2. Register\n";

        std::cout << "Enter your choice: ";
        std::cin >> choice_str;
        std::regex pattern("[1-3]");
        if (std::regex_match(choice_str, pattern))
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice, Enter choice again !" << std::endl;
        }
    }
    int choice = std::stoi(choice_str);
    // std::cout << __LINE__ << __FUNCTION__ << "choice: " << choice << std::endl;

    if (choice == 1)
    {
        str_choice = LOGIN;
    }
    else if (choice == 2)
    {
        str_choice = REGISTER;
    }

    send(clientSocket, str_choice.c_str(), strlen(str_choice.c_str()), 0);
    switch (choice)
    {
    case 1:
        login();
        break;
    case 2:
        registerUser();
        break;
        // exit(EXIT_SUCCESS);
    default:
        std::cerr << "Invalid choice. Please try again.\n";
        break;
    }
}

void Client::login()
{
    // std::cout << __LINE__ << __FUNCTION__ << std::endl;

    if (!isConnected)
    {
        // Tạo socket mới
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (clientSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            outfile << "Error creating socket when login\n";

            exit(EXIT_FAILURE);
        }

        // Kết nối đến server
        if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        {
            std::cerr << "Error connecting to server\n";
            outfile << "Error creating socket when login\n";
            close(clientSocket);
            return;
        }

        isConnected = true;

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        recv(clientSocket, buffer, sizeof(buffer), 0);
    }

    std::cout << "Enter username: ";
    std::string username;
    std::cin >> username;

    std::cout << "Enter password: ";
    std::string password;
    std::cin >> password;
    // std::cout << __LINE__ << std::endl;

    // // std::cout << __LINE__ << std::endl;
    // Send username and password to the server
    send(clientSocket, (username + ":" + password).c_str(), strlen((username + ":" + password).c_str()), 0);
    // std::cout << __LINE__ << std::endl;

    char buffer[1024];
    // std::cout << __LINE__ << std::endl;
    memset(buffer, 0, sizeof(buffer));
    // std::cout << __LINE__ << std::endl;

    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << buffer << "\n";
    std::string checkStr(buffer);
    if (checkStr == "Ok")
    {
        isLogin = true;
        if (username == "admin" && password == "admin")
        {
            isAdmin = true;
            std::cout << "Welcome exam system with run Admintrator \n";
        }
        else
        {
            std::cout << "Welcome exam system with Candidates \n";
        }

        std::cout << "Server response: " << buffer << std::endl;
        showSubMenu();
    }
    else
    {
        std::cout << "id or pw incorrect \n";
        showMenu();
    }

    std::cout << "Server response login: " << buffer << std::endl;
    outfile << "__LOGIN__"
            << "Server response login: " << buffer << std::endl;
}

void Client::registerUser()
{
    std::string newUsername;
    while (true)
    {
        std::cout << "Enter new username: ";
        std::cin >> newUsername;
        if (newUsername == "admin")
        {
            std::cout << "This name cannot be registered, Please enter new user\n";
        }
        else
        {
            break;
        }
    }
    std::cout << "Enter new password: ";
    std::string newPassword;
    std::cin >> newPassword;

    // Send new username and password to the server
    // std::cout << strlen((newUsername + ":" + newPassword).c_str()) << std::endl;
    send(clientSocket, (newUsername + ":" + newPassword).c_str(), strlen((newUsername + ":" + newPassword).c_str()), 0);
    // Receive server response
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response: " << buffer << std::endl;
    outfile << "__registerUser __"
            << "Server response: " << buffer << std::endl;
}

void Client::joinRoom()
{
    int _size;
    recv(clientSocket, &_size, sizeof(_size), 0);
    std::vector<Total_room> alreadyRoom;
    for (int i = 0; i < _size; ++i)
    {
        Total_room room;
        recv(clientSocket, &room, sizeof(room), 0);
        alreadyRoom.push_back(room);
    }
    std::cout << "alreadyRoom.size(): "
              << " : " << alreadyRoom.size() << "_size: " << _size << std::endl;

    std::cout << "List room name available: " << std::endl;

    for (long unsigned int i = 0; i < alreadyRoom.size(); i++)
    {
        std::string strname(alreadyRoom[i].name);
        std::cout << "Name Room: " << strname << std::endl;
        std::cout << "Topic: " << alreadyRoom[i].topic << std::endl;
        // std::cout << "Level: " << strname << std::endl;
        std::cout << "===========================\n";
    }

    if (_size)
    {
        std::string joinRoom;
        std::cout << "Enter room name: ";
        std::cin >> joinRoom;
        // bool isRoomname = false;
        int numberQuestion, timeDuratiron;
        bool isRoom = false;
        for (long unsigned int i = 0; i < alreadyRoom.size(); i++)
        {
            if (strcmp(alreadyRoom[i].name, joinRoom.c_str()) == 0)
            {
                isRoom = true;
                // numberQuestion = alreadyRoom[i].numberQuestion;
                numberQuestion = alreadyRoom[i].easy + alreadyRoom[i].normal + alreadyRoom[i].difficult + alreadyRoom[i].veryhard;

                timeDuratiron = alreadyRoom[i].timeDuration;
                // std::cout << "|numberQuestion: " << numberQuestion << " "
                //           << "|timeDuratiron: " << timeDuratiron << "\n";
                send(clientSocket, joinRoom.c_str(), strlen(joinRoom.c_str()), 0);
                std::string nameUser;
                std::cout << "Please enter name participant: ";
                std::cin >> nameUser;
                send(clientSocket, nameUser.c_str(), strlen(nameUser.c_str()), 0);

                std::cout << "Join room Successfull" << std::endl;
                outfile << "Join room Successfull" << std::endl;

                std::string choice_str;
                while (true)
                {
                    std::cout << "Are you start exam ? " << std::endl;
                    std::cout << "choice 1 for Yes\n";
                    std::cout << "choice 2 for No\n";
                    std::cin >> choice_str;
                    std::regex pattern("[1-2]");
                    if (std::regex_match(choice_str, pattern))
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Invalid choice, Enter choice again !" << std::endl;
                    }
                }
                int choice = std::stoi(choice_str);
                send(clientSocket, &choice, sizeof(choice), 0);
                if (choice == 1)
                {
                    send(clientSocket, &choice, sizeof(choice), 0);
                    std::string topic(alreadyRoom[i].topic);
                    //     // char buffStr[1024];
                    //     // memset(buffStr, 0, sizeof(buffStr));
                    //     // recv(clientSocket, &buffStr, sizeof(buffStr), 0);
                    //     int size_p;
                    //     recv(clientSocket, &size_p, sizeof(size_p), 0);
                    //     int size_s = 2  ;
                    //     recv(clientSocket, &size_s, sizeof(size_s), 0);

                    //     std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << size_s << "|topic:" << topic << std::endl;

                    startExam(joinRoom, numberQuestion, timeDuratiron, topic, alreadyRoom[i].easy,
                              alreadyRoom[i].normal, alreadyRoom[i].difficult, alreadyRoom[i].veryhard);
                    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

                    return;
                }
                else if (choice == 2)
                {
                    return;
                }
                else
                {
                    std::cout << "Please 1 or 2 ";
                }
                break;
            }
        }
        if (!isRoom)
        {
            std::cout << "There's no room with that name ";

            send(clientSocket, RETURN_MAIN_MENU, sizeof(RETURN_MAIN_MENU), 0);
        }
    }
    else
    {
        send(clientSocket, RETURN_MAIN_MENU, sizeof(RETURN_MAIN_MENU), 0);
        std::cout << "No room available\n";
    }
}
void Client::startExam(std::string joinRoom, int numberQuestion, int timeDuration, std::string topic, int easy, int normal, int difficult, int veryhard)
{
    int _size;
    recv(clientSocket, &_size, sizeof(_size), 0);
    std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << _size << "|topic:" << topic << std::endl;

    std::vector<Question> question;
    int easy_count = 0;
    int normal_count = 0;
    int difficult_count = 0;
    int veryhard_count = 0;
    int count = 0;
    for (int i = 0; i < 10; ++i)
    {
        count++;
        Question ques;
        recv(clientSocket, &ques, sizeof(ques), 0);
        // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << _size << std::endl;
        if ((strcmp(ques.topic, topic.c_str()) == 0))
        {
            // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << _size << std::endl

            if (ques.level == 1 && easy_count < easy)
            {
                question.push_back(ques);
                easy_count++;
            }
            else if (ques.level == 2 && normal_count < normal)
            {
                question.push_back(ques);
                normal_count++;
            }
            else if (ques.level == 3 && difficult_count < difficult)
            {
                question.push_back(ques);
                difficult_count++;
            }
            else if (ques.level == 2 && veryhard_count < veryhard)
            {
                question.push_back(ques);
                veryhard_count++;
            }
        }
    }
    // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << question.size()
    //           << "|timdur: " << timeDuration << "|Numberquestion: " << numberQuestion << std::endl;
    std::cout << __LINE__ << " : " << __FUNCTION__
              << "|count: " << count << std::endl;

    numberQuestion = easy + normal + difficult + veryhard;
    int correctAnswers = 0;
    // std::cout << "Number of correct answers: " << correctAnswers << "/" << numberQuestion << std::endl;
    showTest(question, timeDuration, numberQuestion, correctAnswers, false);
    std::cout << "Number of correct answers: " << correctAnswers << "/" << numberQuestion << std::endl;

    outfile << "Number of correct answers: " << correctAnswers << "/" << numberQuestion << std::endl;
    int score = ((float)correctAnswers / numberQuestion) * 10;
    std::cout << "Your score: " << score << std::endl;
    // std::cout << "joinRoom: " << joinRoom  << std::endl;
    send(clientSocket, &score, sizeof(score), 0);
    std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;
    return;

    // DbSqlite::getInstance()->set_score_room(score, joinRoom);
}
void Client::viewStatusRoom()
{
    int size_vec;
    std::vector<Room> roomInfo;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ <<" : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (int i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Room room;
        recv(clientSocket, &room, sizeof(room), 0);
        roomInfo.push_back(room);
    }
    for (long unsigned int i = 0; i < roomInfo.size(); i++)
    {
        std::cout << "|Name room: " << roomInfo[i].name << " ";
        if (roomInfo[i].status == 0)
        {
            std::cout << "|Status room :"
                      << "Already for test\n";
            outfile << "|Status room :"
                    << "Already for test\n";
        }
        else if (roomInfo[i].status == 1)
        {
            std::cout << "|Status room :"
                      << "Testing\n";
            outfile << "|Status room :"
                    << "Testing\n";
        }
        else if (roomInfo[i].status == 2)
        {
            std::cout << "|Status room :"
                      << "Ending\n";
            outfile << "|Status room :"
                    << "Ending\n";
        }
    }
}

void Client::resultRoom()
{
    std::cout << "Result of Room: ";
    int size_vec;
    std::vector<Room_result> roomInfo;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (int i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Room_result room;
        recv(clientSocket, &room, sizeof(room), 0);
        roomInfo.push_back(room);
    }
    std::cout << "Result Room: " << std::endl;
    std::cout << "================================\n";
    for (long unsigned int i = 0; i < roomInfo.size(); i++)
    {
        std::cout << "|Room Name: " << roomInfo[i].name << ": "
                  << "\nRoom user: " << roomInfo[i].user << ": "
                  << "\nRoom score: " << roomInfo[i].score << std::endl;
        std::cout << "================================\n";
        outfile << "Result Room: " << std::endl;
        outfile << roomInfo[i].name << ": " << roomInfo[i].user << ": "
                << roomInfo[i].score << std::endl;
    }
}
void Client::trainingMode()
{
    int correctAnswer;
    outfile << "Welcome training Mode \n";
    std::cout << "Welcome training Mode \n";
    int size_vec;
    std::vector<Question> question;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (int i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Question ques;
        recv(clientSocket, &ques, sizeof(ques), 0);
        question.push_back(ques);
    }

    showTest(question, NUMBER_QUESTION_TRAINING_MODE * TIME_FOR_EACH_QUESTION, NUMBER_QUESTION_TRAINING_MODE, correctAnswer, true);
    std::cout << "Number of correct answers: " << correctAnswer << "/" << NUMBER_QUESTION_TRAINING_MODE << std::endl;
    int score = ((float)correctAnswer / NUMBER_QUESTION_TRAINING_MODE) * 10;
    std::cout << "Your score: " << score << std::endl;
    outfile << "Number of correct answers: " << correctAnswer << "/" << NUMBER_QUESTION_TRAINING_MODE << std::endl;
    showSubMenu();
}
void Client::viewLogTrace()
{
    std::ifstream infile("LOG.txt");
    if (infile.is_open())
    {
        std::string line;
        while (std::getline(infile, line))
        {
            std::cout << line << std::endl;
        }

        infile.close();
    }
    else
    {
        std::cerr << "Can not open file" << std::endl;
    }
}

void Client::Logout()
{
    isLogin = false;
    isAdmin = false;
    showMenu();
}
// void Client::createNewRoom(int clientSocket)
// {
//     std::string newRoom;
//     std::cout << "Enter new room name: ";
//     std::cin >> newRoom;
//     send(clientSocket, newRoom.c_str(), strlen(newRoom.c_str()), 0);

//     Room_info roominfo;

//     std::string timerDurationStr;
//     while (true)
//     {
//         std::cout << "Enter time duration: ";
//         // std::cin >> roominfo.timeDuration;
//         std::cin >> timerDurationStr;
//         std::regex pattern("^[0-9]+$");
//         if (std::regex_match(timerDurationStr, pattern))
//         {
//             break;
//         }
//         else
//         {
//             std::cout << "Invalid choice, Enter choice again !" << std::endl;
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }
//     }

//     roominfo.timeDuration = std::stoi(timerDurationStr);

//     std::cout << "Enter Topic: ";
//     std::cin >> roominfo.topic;

//     std::string easeStr;
//     while (true)
//     {
//         std::cout << "Enter new number of question  for level easy : ";
//         // std::cin >> roominfo.timeDuration;
//         std::cin >> easeStr;
//         std::regex pattern("^[0-9]+$");
//         if (std::regex_match(easeStr, pattern))
//         {
//             break;
//         }
//         else
//         {
//             std::cout << "Invalid choice, Enter choice again !" << std::endl;
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }
//     }
//     roominfo.easy = std::stoi(easeStr);
//     std::string normalStr;
//     while (true)
//     {
//         std::cout << "Enter new number of question  for level normal : ";
//         // std::cin >> roominfo.timeDuration;
//         std::cin >> normalStr;
//         std::regex pattern("^[0-9]+$");
//         if (std::regex_match(normalStr, pattern))
//         {
//             break;
//         }
//         else
//         {
//             std::cout << "Invalid choice, Enter choice again !" << std::endl;
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }
//     }
//     roominfo.normal = std::stoi(normalStr);
//     std::string difficultStr;
//     while (true)
//     {
//         std::cout << "Enter new number of question  for level difficult : ";
//         // std::cin >> roominfo.timeDuration;
//         std::cin >> difficultStr;
//         std::regex pattern("^[0-9]+$");
//         if (std::regex_match(difficultStr, pattern))
//         {
//             break;
//         }
//         else
//         {
//             std::cout << "Invalid choice, Enter choice again !" << std::endl;
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }
//     }
//     roominfo.difficult = std::stoi(difficultStr);
//     std::string veryhardStr;
//     while (true)
//     {
//         std::cout << "Enter new number of question  for level veryhard : ";
//         // std::cin >> roominfo.timeDuration;
//         std::cin >> veryhardStr;
//         std::regex pattern("^[0-9]+$");
//         if (std::regex_match(veryhardStr, pattern))
//         {
//             break;
//         }
//         else
//         {
//             std::cout << "Invalid choice, Enter choice again !" << std::endl;
//             std::cin.clear();
//             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//         }
//     }
//     roominfo.veryhard = std::stoi(veryhardStr);

//     roominfo.numberQuestion = roominfo.easy + roominfo.normal + roominfo.veryhard + roominfo.difficult;

//     roominfo.score = 0;
//     roominfo.status = 0;
//     if (send(clientSocket, &roominfo, sizeof(roominfo), 0) == -1)
//     {
//         std::cerr << "Error sending data to the server.\n";
//         close(clientSocket);
//         return;
//     }
//     std::cout << "Create New Room OK";
//     outfile << "Create New Room OK";

// }