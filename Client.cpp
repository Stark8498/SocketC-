#include "Client.h"

// function
std::vector<std::string> splitString(const std::string &inputString, char delimiter)
{
    std::istringstream iss(inputString);
    std::vector<std::string> tokens;

    std::string token;
    while (std::getline(iss, token, delimiter))
    {
        size_t start = token.find_first_not_of(" ");
        size_t end = token.find_last_not_of(" ");
        if (start != std::string::npos && end != std::string::npos)
        {
            tokens.push_back(token.substr(start, end - start + 1));
        }
    }

    return tokens;
}

void timerFunction(int duration, std::atomic<bool> &flag)
{
    auto start = std::chrono::steady_clock::now();
    while (true)
    {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
        if (elapsed.count() >= duration)
        {
            flag.store(false, std::memory_order_relaxed);
            break;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Client::showTest(std::vector<Question> question, int timeDuration, int numberQuestion)
{
    srand(time(0));

    // Question questions[5];
    // std::cout << "Start Question Test"
    //           << "|numberQuestion: " << numberQuestion << "|timeDuration: " << timeDuration << std::endl;

    time_t startTime = time(0);
    time_t elapsedTime;

    int correctAnswers = 0;
    while (true)
    {
        // // std::cout << __LINE__ << ": " << __FUNCTION__ << "\n";

        elapsedTime = time(0) - startTime;
        if (elapsedTime >= timeDuration)
        {
            std::cout << "End Time ! END TEST" << std::endl;
            break;
        }
        if (numberQuestion > question.size())
        {
            numberQuestion = question.size();
        }
        if (numberQuestion == 0)
        {
            std::cout << "There are no matching questions in the DB \n";
            outfile << "There are no matching questions in the DB \n";

            break;
        }

        for (size_t i = 0; i < numberQuestion; i++)
        {
            std::string userAnswer = askQuestion(question[i]);

            if (strcmp(question[i].correctAnswer, userAnswer.c_str()) == 0)
            {
                // std::cout << "Right!" << std::endl;
                correctAnswers++;
            }

            // sleep(2);
        }
        std::cout << "What question do you still have time to change your answer to? " << std::endl;
        while (true)
        {
            std::cout << "Are you change your answer exam ? " << std::endl;
            std::cout << "choice 1 for Yes\n";
            std::cout << "choice 2 for No, Summit exam\n";
            int choice_change;
            std::cin >> choice_change;
            if (choice_change == 1)
            {
                int number;
                std::cout << "Number question change: ";
                std::cin >> number;
                std::cout << "Chọn đáp án (A-B): ";
                std::string answ;
                std::cin >> answ;
                if (strcmp(question[number].correctAnswer, answ.c_str()) == 0)
                {
                    correctAnswers++;
                }
                // break;
            }
            else if (choice_change == 2)
            {
                break;
            }
            else
            {
                std::cout << "Please 1 or 2 ";
            }
        }
        break;
    }
    std::cout << "Number of correct answers: " << correctAnswers << "/" << numberQuestion << std::endl;
    outfile << "Number of correct answers: " << correctAnswers << "/" << numberQuestion << std::endl;
    return;
}

std::string Client::askQuestion(Question q)
{
    std::cout << q.content << std::endl;
    std::cout << q.choices1 << std::endl;
    std::cout << q.choices2 << std::endl;
    std::cout << q.choices3 << std::endl;
    std::cout << q.choices4 << std::endl;
    std::string userAnswer;
    std::cout << "Choose answer (A-B): ";
    std::cin >> userAnswer;

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
    int choice;
    switch (isAdmin)
    {
    case true:
    {
        while (true)
        {
            std::cout << "\n===== Menu =====\n";
            std::cout << "1. Create new exam room\n";
            std::cout << "2. Update_durationn\n";
            std::cout << "3. View Trace Log System\n";
            std::cout << "4. Join_rom\n";
            std::cout << "5. View_status_room\n";
            std::cout << "6. View_result_room\n";
            std::cout << "7. Training_mode\n";
            std::cout << "8. Logout\n";


            std::cout << "Enter your choice: ";
            std::cin >> choice;
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

            if (str_choice.length() != 0)
            {
                send(clientSocket, str_choice.c_str(), strlen(str_choice.c_str()), 0);
            }
            switch (choice)
            {

            case 1:
                createExamRoom();
                break;
            case 2:
            {
                setExamDuration();
                break;
            }
            case 3:
                viewLogTrace();
                break;
            case 4:
                joinRoom();
                break;
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
            std::cout << "Enter your choice: ";
            std::cin >> choice;
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
            default:
                std::cerr << "Invalid choice. Please try again.\n";
                break;
            }
        }

        break;
        break;
    }
    }
}
void Client::createExamRoom()
{
    // std::cout << "|Insight create Room";
    Room roominfo;

    std::cout << "Enter new room name: ";
    std::cin >> roominfo.name;

    std::cout << "Enter new user name: ";
    std::cin >> roominfo.user;

    std::cout << "Enter time duration: ";
    std::cin >> roominfo.timeDuration;

    std::cout << "Enter new number of question : ";
    std::cin >> roominfo.numberQuestion;

    std::cout << "Enter Topic: ";

    std::cin >> roominfo.topic;
    std::cout << "Enter level: ";

    int levelcin;
    std::cin >> levelcin;
    do
    {
        if (levelcin != 0 && levelcin != 1 &&
            levelcin != 2 && levelcin != 3)
        {
            std::cout << "Please choice 0-3" << std::endl;
        }

        std::cout << "Choose level " << std::endl;
        std::cout << "0. easy\n";
        std::cout << "1. normal\n";
        std::cout << "2. difficult\n";
        std::cout << "3. veryhard\n";
        std::cout << "Enter level: ";
    } while (levelcin != 0 && levelcin != 1 &&
             levelcin != 2 && levelcin != 3);

    roominfo.score = 0;
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
    int numberofQuestion;
    std::cout << "Enter numberOfQuestion: ";
    std::cin >> numberofQuestion;
    send(clientSocket, &numberofQuestion, sizeof(numberofQuestion), 0);
    std::cout << "Set timeduration OK";
    outfile << "Set timeduration OK";
}

void Client::setExamDuration()
{
    // std::cout
    std::cout << "Please choose room: ";
    int timeDuration;
    Room room;
    std::vector<Room> roomInfo;
    int _size;
    recv(clientSocket, &_size, sizeof(_size), 0);
    recv(clientSocket, &roomInfo[0], _size * sizeof(roomInfo), 0);
    for (size_t i = 0; i < _size; i++)
    {
        std::cout << roomInfo[i].name << ": ";
    }
    std::cout << "\nchoose room: ";
    std::cin >> room.name;
    // send(clientSocket, room.name, sizeof(room.name), 0);
    std::cout << "Enter timeDuration: ";
    std::cin >> room.timeDuration;
    send(clientSocket, &room, sizeof(room), 0);
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
    int choice;
    char buffer_choice[1024] = {9};
    while (true)
    {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Log in\n";
        std::cout << "2. Register\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::string str_choice;
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
}

void Client::login()
{
    // // std::cout << __LINE__ << std::endl;

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
    std::cout << "Enter new username: ";
    std::string newUsername;
    std::cin >> newUsername;
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
    std::vector<Room> alreadyRoom;
    for (size_t i = 0; i < _size; ++i)
    {
        Room room;
        recv(clientSocket, &room, sizeof(Room), 0);
        alreadyRoom.push_back(room);
    }

    std::cout << "alreadyRoom.size(): "
              << " : " << alreadyRoom.size() << "_size: " << _size << std::endl;

    std::cout << "List room name available: " << std::endl;

    for (size_t i = 0; i < alreadyRoom.size(); i++)
    {
        std::string strname(alreadyRoom[i].name);
        std::cout << "Name Room: " << strname << std::endl;
        std::cout << "Topic: " << strname << std::endl;
        std::cout << "Level: " << strname << std::endl;
        std::cout << "===========================\n";
    }

    if (_size)
    {
        std::string joinRoom;
        std::cout << "Enter room name: ";
        std::cin >> joinRoom;
        bool isRoomname = false;
        int numberQuestion, timeDuratiron;
        bool isRoom = false;
        for (size_t i = 0; i < alreadyRoom.size(); i++)
        {
            if (strcmp(alreadyRoom[i].name, joinRoom.c_str()) == 0)
            {
                isRoom = true;
                numberQuestion = alreadyRoom[i].numberQuestion;
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

                while (true)
                {
                    std::cout << "Are you start exam ? " << std::endl;
                    std::cout << "choice 1 for Yes\n";
                    std::cout << "choice 2 for No\n";
                    int choice;
                    std::cin >> choice;
                    send(clientSocket, &choice, sizeof(choice), 0);
                    if (choice == 1)
                    {
                        startExam(numberQuestion, timeDuratiron, alreadyRoom[i].topic, alreadyRoom[i].level);
                        break;
                    }
                    else if (choice == 2)
                    {
                        break;
                    }
                    else
                    {
                        std::cout << "Please 1 or 2 ";
                    }
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
void Client::startExam(int numberQuestion, int timeDuration, char topic[100], int level)
{
    std::vector<Question> levelquestion;
    int _size;
    recv(clientSocket, &_size, sizeof(_size), 0);

    // std::vector<Question> question(_size);
    // recv(clientSocket, &question[0], _size * sizeof(question), 0);

    std::vector<Question> question;
    for (size_t i = 0; i < _size; ++i)
    {
        Question ques;
        recv(clientSocket, &ques, sizeof(Question), 0);
        question.push_back(ques);
    }
    for (int i = 0; i < question.size(); i++)
    {
        if (question[i].level == level && (strcmp(question[i].topic, topic) == 0))
        {
            levelquestion.push_back(question[i]);
        }
    }
    showTest(levelquestion, timeDuration, numberQuestion);
}
void Client::viewStatusRoom()
{
    int size_vec;
    std::vector<Room> roomInfo;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ <<" : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (size_t i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Room room;
        recv(clientSocket, &room, sizeof(room), 0);
        roomInfo.push_back(room);
    }
    for (size_t i = 0; i < roomInfo.size(); i++)
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
    std::vector<Room> roomInfo;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (size_t i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Room room;
        recv(clientSocket, &room, sizeof(room), 0);
        roomInfo.push_back(room);
    }
    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        if (roomInfo[i].status == 3)
        {
            std::cout << "Result Room: " << std::endl;
            std::cout << roomInfo[i].name << ": " << roomInfo[i].user << ": "
                      << roomInfo[i].score << std::endl;
            outfile << "Result Room: " << std::endl;
            outfile << roomInfo[i].name << ": " << roomInfo[i].user << ": "
                    << roomInfo[i].score << std::endl;
        }
    }
    std::cout << "| These no room ending and do not view result\n";
    outfile << "| These no room ending and do not view result\n";
}
void Client::trainingMode()
{
    outfile << "Welcome training Mode \n";
    std::cout << "Welcome training Mode \n";
    int size_vec;
    std::vector<Question> question;
    recv(clientSocket, &size_vec, sizeof(size_vec), 0);
    // std::cout << __LINE__ << " : " << __FUNCTION__ << "|size_vec: " << size_vec << std::endl;

    for (size_t i = 0; i < size_vec; i++)
    {
        // std::cout << __LINE__ << " : " << __FUNCTION__ << std::endl;

        Question ques;
        recv(clientSocket, &ques, sizeof(ques), 0);
        question.push_back(ques);
    }

    showTest(question, NUMBER_QUESTION_TRAINING_MODE * TIME_FOR_EACH_QUESTION, NUMBER_QUESTION_TRAINING_MODE);
    showSubMenu();
}
void Client::updateEndTime()
{
    Room room;
    std::cout << "Enter name room: ";
    std::cin >> room.name;
    std::cout << "Enter time duration: ";
    std::cin >> room.timeDuration;
    send(clientSocket, &room, sizeof(room), 0);
    outfile << "Update end time room: " << room.name << "Successfull\n";
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