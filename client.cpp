#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <thread>

const int MESSAGE_LENGTH = 1024;
char message[MESSAGE_LENGTH];
int clientSocket;

// Function to handle user input and send messages to the server
void handleUserInput() {
    while (true) {
        memset((char *)&message, 0, sizeof(message));
        std::cin.getline(message, MESSAGE_LENGTH);

        send(clientSocket, message, strlen(message), 0);

        // Check if the user wants to exit
        if (strcmp(message, "/exit") == 0) {
            close(clientSocket);
            break;
        }
    }
}

// Function to receive messages from the server
void receiveMessages() {
    while (true) {
        memset((char *)&message, 0, sizeof(message));
        recv(clientSocket, message, sizeof(message), 0);

        // Check for server disconnection or empty message
        if (strlen(message) == 0 || strcmp(message, "Server Disconnected") == 0) {
            std::cout << message << std::endl;
            break;
        }

        // Print received message with yellow text
        std::cout << "\x1B[33m" << message << "\x1B[0m" << std::endl;
    }

    // Close the client socket and exit the program
    close(clientSocket);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <Server_IP_Address> <Server_Port_Number>\n", argv[0]);
        return 1;
    }

    const char *serverIP = argv[1];
    int serverPort = atoi(argv[2]);

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cout << "Failed to create client socket" << std::endl;
        return 1;
    }

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);
    serverAddress.sin_addr.s_addr = inet_addr(serverIP);

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cout << "\x1B[31m" << "Failed to connect to the server" << "\x1B[0m" << std::endl;
        return 1;
    }

    std::cout << "\x1B[34m" << "Connected to the server\n" << "\x1B[0m" << std::endl;

    // Start a thread to receive messages and another to handle user input
    std::thread(receiveMessages).detach();
    std::thread(handleUserInput).join();

    return 0;
}
