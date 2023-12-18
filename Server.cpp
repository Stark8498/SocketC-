// Server.cpp
#include "Server.h"
#include "SqliteDatabase.h"

Server::Server() : isConnected(true){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8000);

    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding socket\n";
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 10) == -1) {
        std::cerr << "Error listening on socket\n";
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port 8888...\n";
}

Server::~Server() {
    close(serverSocket);
}

void Server::start() {
    while (isConnected) {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (!isConnected) {
            // Nếu kết nối đã đóng, thoát khỏi vòng lặp
            break;
        }
        // Send confirmation to the client
        const char* confirmationMessage = "Connection established. Welcome to the server!";
        send(clientSocket, confirmationMessage, strlen(confirmationMessage), 0);

        std::cout << "Sent confirmation to client\n";

        // Handle other client requests here...

        // Receive client request type
        while (true)
        {
            char requestType[1024] ={0};
            memset(requestType, 0, 1024);

            recv(clientSocket, &requestType, sizeof(requestType)-1, 0);
            std::string str_req(requestType);

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
                handleRegistration(clientSocket);
            }
            else if (str_req == UPDATE_DURATION)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == UPDATE_TIME_END_ROOM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == JOIN_ROOM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == VIEW_STATUS_ROOM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == VIEW_RUSULT_ROOM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == START_EXAM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == SUBMIT_EXAM)
            {
                handleRegistration(clientSocket);
            }
            else if (str_req == TRAINING_MODE)
            {
                handleTrainningMode(clientSocket);
            }
            else if (str_req == NUMBER_QUESTION_TRAINING_MODE)
            {
                /* code */
            }
            
        }       
        close(clientSocket);
    }
}


void Server::handleLogin(int clientSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    // Receive username and password from the client    
    recv(clientSocket, buffer, sizeof(buffer), 0);
    // Extract username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    std::string username = credentials.substr(0, pos);
    std::string password = credentials.substr(pos + 1);
    if (SqliteDatabase::getInstance().getIdUser(username, password) != -1 )
    {
        const char* successMessage = "Ok";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else
    {
        const char* errorMessage = "NotOk";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

void Server::handleRegistration(int clientSocket) {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));

    // Receive new username and password from the client
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "|tin nhan tu regis server: " << buffer << std::endl;

    // Extract new username and password from the received string
    std::string credentials(buffer);
    size_t pos = credentials.find(':');
    std::string newUsername = credentials.substr(0, pos);
    std::string newPassword = credentials.substr(pos + 1);
     if (SqliteDatabase::getInstance().inserIntoTableUser(newUsername, newPassword))
    {
        const char* successMessage = "Registration successful";
        send(clientSocket, successMessage, strlen(successMessage), 0);
    }
    else
    {
        const char* errorMessage = "Registration failed";
        send(clientSocket, errorMessage, strlen(errorMessage), 0);
    }
}

void Server::handleCreateExamRoom(int clientSocket) {
    std::cout << "Handling create exam room request\n";
    // Create new room: name, test time  numberquestion, timestart
    const char* responseMessage = "Exam room created successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void Server::handleSetNumberOfQuestions(int clientSocket) {
    std::cout << "Handling set number of questions request\n";
    //insert number question
    const char* responseMessage = "Number of questions set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}

void Server::handleSetExamDuration(int clientSocket) {
    std::cout << "Handling set exam duration request\n";
    //insert number durration
    const char* responseMessage = "Exam duration set successfully";
    send(clientSocket, responseMessage, strlen(responseMessage), 0);
}
void Server::handleTrainningMode(int clientSocket)
{
    std::vector<Question> question = SqliteDatabase::getInstance().getQuestionTraining();
    send(clientSocket, &question[0], question.size() * sizeof(question), 0); 

}


