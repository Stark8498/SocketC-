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

void Client::showTest(std::vector<Question> question, int timeDuration)
{
    srand(time(0));

    // Question questions[5];
    std::cout << "Start Question Test" << std::endl;

    time_t startTime = time(0);
    time_t elapsedTime;

    int correctAnswers = 0;
    while (true)
    {
        elapsedTime = time(0) - startTime;
        if (elapsedTime >= timeDuration)
        {
            std::cout << "End Time ! END TEST" << std::endl;
            break;
        }
        for (size_t i = 0; i < question.size(); i++)
        {
            int userAnswer = askQuestion(question[i]);

            if (userAnswer == question[i].correctAnswer)
            {
                std::cout << "Right!" << std::endl;
                correctAnswers++;
            }
            else
            {
                std::cout << "Incorrect. Answer correct is :  " << question[i].correctAnswer << std::endl;
            }
            sleep(2);
        }
    }
    std::cout << "Number of correct answers: " << correctAnswers << "/5" << std::endl;
}

int Client::askQuestion(Question q)
{
    std::cout << q.content << std::endl;
    std::cout << q.choices1 << std::endl;
    std::cout << q.choices2 << std::endl;
    std::cout << q.choices3 << std::endl;
    std::cout << q.choices4 << std::endl;
    int userAnswer;
    std::cout << "Chọn đáp án (1-4): ";
    std::cin >> userAnswer;

    return userAnswer;
}

Client::Client() : isConnected(false)
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (clientSocket == -1)
    {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8000);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
}

Client::~Client()
{
    disconnect();
}

void Client::showSubMenu()
{
    int choice;

    while (true)
    {
        std::cout << "\n===== Menu =====\n";
        std::cout << "1. Create new exam room\n";
        std::cout << "2. update_durationn";
        std::cout << "3. update_time_end_room\n";
        std::cout << "4. join_rom";
        std::cout << "5. view_status_room";
        std::cout << "6. view_result_room\n";
        std::cout << "7. training_mode\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
    }
    std::string str_choice;
    if (choice == 1)
    {
        str_choice = CREATE_ROOM;
    }
    else if (choice == 2)
    {
        str_choice == UPDATE_DURATION;
    }
    else if (choice == 3)
    {
        str_choice == UPDATE_TIME_END_ROOM;
    }
    else if (choice == 4)
    {
        str_choice == JOIN_ROOM;
    }
    else if (choice == 5)
    {
        str_choice == VIEW_STATUS_ROOM;
    }
    else if (choice == 6)
    {
        str_choice == VIEW_RUSULT_ROOM;
    }
    else if (choice == 7)
    {
        str_choice == TRAINING_MODE;
    }
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
        setExamDuration();
        break;
    case 3:
        updateEndTime();
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
    default:
        std::cerr << "Invalid choice. Please try again.\n";
        break;
    }
}
void Client::createExamRoom()
{
    Room roominfo;

    std::cout << "Enter new room name: ";
    std::cin >> roominfo.name;

    std::cout << "Enter time duration: ";
    std::cin >> roominfo.timeDuration;

    std::cout << "Enter new number of question : ";
    std::cin >> roominfo.numberQuestion;

    send(clientSocket, &roominfo, sizeof(roominfo), 0);
    std::cout << "Crate New Room OK";
}

void Client::setNumberOfQuestions()
{
    int numberofQuestion;
    std::cout << "Enter numberOfQuestion: ";
    std::cin >> numberofQuestion;
    send(clientSocket, &numberofQuestion, sizeof(numberofQuestion), 0);
    std::cout << "Set timeduration OK";
}

void Client::setExamDuration()
{

    int timeDuration;
    std::cout << "Enter timeDuration: ";
    std::cin >> timeDuration;
    send(clientSocket, &timeDuration, sizeof(timeDuration), 0);
    std::cout << "Set timeduration OK";
}

void Client::connectToServer()
{
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
    {
        std::cerr << "Error connecting to server\n";
        exit(EXIT_FAILURE);
    }

    isConnected = true;

    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive confirmation from the server
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Server response: " << buffer << std::endl;

    // Show the main menu after successful connection
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
    }
    else
    {
        std::cout << "Thoat chuong trinh";
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
        std::cout << "2. Register1\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        std::string str_choice;
        if (choice == 1)
        {
            str_choice = LOGIN;
        }
        else if (choice == 2)
        {
            str_choice == REGISTER;
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
        case 3:
            disconnect();
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
    std::cout << __LINE__ << std::endl;

    if (!isConnected)
    {
        // Tạo socket mới
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (clientSocket == -1)
        {
            std::cerr << "Error creating socket\n";
            exit(EXIT_FAILURE);
        }

        // Kết nối đến server
        if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
        {
            std::cerr << "Error connecting to server\n";

            // Đóng socket mới nếu kết nối không thành công
            close(clientSocket);
            return;
        }

        isConnected = true;

        char buffer[1024];
        memset(buffer, 0, sizeof(buffer));

        // Nhận phản hồi từ server
        recv(clientSocket, buffer, sizeof(buffer), 0);
    }

    std::cout << "Enter username: ";
    std::string username;
    std::cin >> username;

    std::cout << "Enter password: ";
    std::string password;
    std::cin >> password;

    // Send request type
    // int requestType = 1;
    std::cout << __LINE__ << std::endl;

    // send(clientSocket, &requestType, sizeof(requestType), 0);
    std::cout << __LINE__ << std::endl;
    // Send username and password to the server
    send(clientSocket, (username + ":" + password).c_str(), strlen((username + ":" + password).c_str()), 0);
    std::cout << __LINE__ << std::endl;

    // Receive server response
    char buffer[1024];
    std::cout << __LINE__ << std::endl;
    memset(buffer, 0, sizeof(buffer));
    std::cout << __LINE__ << std::endl;

    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::string checkStr(buffer);
    if (checkStr == "Ok")
    {
        std::cout << "Server response: " << buffer << std::endl;
        showSubMenu();
    }

    std::cout << "Server response login: " << buffer << std::endl;
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
    std::cout << strlen((newUsername + ":" + newPassword).c_str()) << std::endl;
    send(clientSocket, (newUsername + ":" + newPassword).c_str(), strlen((newUsername + ":" + newPassword).c_str()), 0);
    // Receive server response
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Server response: " << buffer << std::endl;
}

void Client::joinRoom()
{
    std::vector<Room> alreadyRoom;
    recv(clientSocket, &alreadyRoom[0], alreadyRoom.size() * sizeof(alreadyRoom), 0);
    for (size_t i = 0; i < alreadyRoom.size(); i++)
    {
        std::cout << "List room name available: " << alreadyRoom[i].name  << std::endl;        
    }
    
    std::string joinRoom;
    // getline(std::cin, joinRoom);
    std::cout << "Please choose room: ";
    std::cin >> joinRoom;
    send(clientSocket, joinRoom.c_str(), strlen(joinRoom.c_str()), 0);
    std::string nameUser;
    std::cout << "Please enter name participant: ";
    std::cin >> nameUser;
    send(clientSocket, nameUser.c_str(), strlen(nameUser.c_str()), 0);
    std::cout << "Join room Successfull" << std::endl;
    while (true)
    {
        std::cout << "Are you start exam ? " << std::endl;
        std::cout << "choice 1 for Yes\n"; 
        std::cout << "choice 2 for No\n";
        int choice;
        std::cin >> choice; 
        if (choice == 1)
        {
            int reqRoom = 1;
            send(clientSocket, &reqRoom, sizeof(reqRoom), 0);
            startExam();
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
}
void Client::startExam()
{
    std::vector<Question> question(NUMBER_QUESTION_TRAINING_MODE);
    recv(clientSocket, &question[0], question.size() * sizeof(question), 0);
    int timeDuration;
    recv(clientSocket, &timeDuration, sizeof(timeDuration), 0);
    for (size_t i = 0; i < question.size(); i++)
    {
        showTest(question, timeDuration);
    }
}
void Client::viewStatusRoom()
{
    // view status -> response server is "name server: isable" in
    std::cout << "Status of Room: ";
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Receive username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    // Extract username and password from the received string
    std::string credentials(buffer);
    std::vector<std::string> tokens = splitString(credentials, ',');
    for (const auto &t : tokens)
    {
        std::cout << t << std::endl;
    }
}

void Client::resultRoom()
{
    std::cout << "Result of Room: ";
    int vec_size;
    recv(clientSocket, &vec_size, sizeof(vec_size), 0);
    std::vector<Room> roomInfo;
    recv(clientSocket, &roomInfo[0], vec_size * sizeof(roomInfo), 0 );
    for (size_t i = 0; i < roomInfo.size(); i++)
    {
        if (roomInfo[i].status == 3)
        {
            std::cout << "Result Room: " << std::endl;
            std::cout << roomInfo[i].name << ": " << roomInfo[i].user << ": " 
            << roomInfo[i].score << std::endl;
        }   
    } 
}
void Client::trainingMode()
{
    std::vector<Question> question(NUMBER_QUESTION_TRAINING_MODE);
    recv(clientSocket, &question[0], question.size() * sizeof(question), 0);
    for (size_t i = 0; i < question.size(); i++)
    {
        showTest(question, NUMBER_QUESTION_TRAINING_MODE * TIME_FOR_EACH_QUESTION);
    }
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


}