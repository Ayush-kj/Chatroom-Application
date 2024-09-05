#include <iostream>
#include <cstring>
#include <thread>
#include <map>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <sys/time.h>

const int MESSAGE_LENGTH = 1024;
char message[MESSAGE_LENGTH];
std::map<int, int> clientSockets;

// Function to handle individual client connections
void handleClient(int clientSocket) {
    std::cout << "\x1B[32m" << "Client " << clientSocket << " connected" << "\x1B[0m" << "\n";

    // Send an initial message to the client
    std::string initialMsg = "Client ID: " + std::to_string(clientSocket);
    const char *msgg = initialMsg.data();
    send(clientSocket, msgg, strlen(msgg), 0);

    while (true) {
        int flag = 0;
        char msg[MESSAGE_LENGTH];
        int bytes = recv(clientSocket, msg, sizeof(msg), 0);

        if (bytes == 0 || strcmp(msg, "/exit") == 0) {
            std::cout << "\x1B[31m" << "Client " << clientSocket << " disconnected" << "\x1B[0m" << std::endl;
            clientSockets.erase(clientSocket);

            std::string conMsg1 = (std::string) "\x1B[31m" + "Client " + std::to_string(clientSocket) + " disconnected" + (std::string) "\x1B[0m";
            const char *conMsg = conMsg1.data();

            // Notify other clients about the disconnection
            for (auto x : clientSockets) {
                send(x.first, conMsg, strlen(conMsg), 0);
            }
            break;
        }

        int idx = 0;
        std::string sock_str;
        while (msg[idx] != ' ' && idx < sizeof(msg)) {
            if (msg[idx] < '0' || msg[idx] > '9') {
                flag = 1;
            }
            sock_str += msg[idx];
            idx++;
        }
        idx++;
        std::string actualmsg;
        actualmsg += "Client ";
        actualmsg += std::to_string(clientSocket);
        actualmsg += ": ";
        while (msg[idx] < sizeof(msg) && msg[idx] != '\0') {
            actualmsg += msg[idx];
            idx++;
        }
        const char *msgg = actualmsg.data();
        int socket_id = 0;
        if (flag == 0) {
            socket_id = std::stoi(sock_str);
        }
        if (sock_str == "1") {
            // Broadcast the message to all clients
            for (auto x : clientSockets) {
                send(x.first, msgg, strlen(msgg), 0);
            }
        } else {
            if (clientSockets.count(socket_id) && socket_id != clientSocket) {
                // Send the message to a specific client
                send(socket_id, msgg, strlen(msgg), 0);
            } else {
                flag = 1;
            }
        }
        if (flag == 1) {
            msgg = "Please use the correct format: <clientNo.> <Msg>";
            send(clientSocket, msgg, strlen(msgg), 0);
        }
    }
    close(clientSocket);
    clientSockets.erase(clientSocket);
}

// Function to handle user input (server shutdown)
void handleInput() {
    while (true) {
        memset((char *)&message, 0, sizeof(message));
        std::cin.getline(message, MESSAGE_LENGTH);
        if (strcmp(message, "/exit") == 0 || strlen(message) == 0) {
            const char *disconMsg = "\x1b[31mServer Disconnected \x1b[0m";
            for (auto x : clientSockets) {
                send(x.first, disconMsg, strlen(disconMsg), 0);
            }
            clientSockets.clear();
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <serverPort>" << std::endl;
        return 1;
    }

    uint PORT = std::stoi(argv[1]);
    const char *Server_IP = "127.0.0.1";

    int max_clients = 30;
    int opt = 1;
    int master_socket, addrlen, new_socket = 0, i, valread, socket_descriptor_id;
    int max_socket_descriptor_id;
    struct sockaddr_in address;
    char message_buffer[MESSAGE_LENGTH];
    fd_set readfds;
    const char *message = "Welcome Client! \r\n";

    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "socket System Call Error: Failed to create Socket" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) {
        std::cout << "setsockopt System Call Error" << std::endl;
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(Server_IP);
    address.sin_port = htons(PORT);

    if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0) {
        std::cout << "bind System Call Error: Failed to bind socket with Server_ID and Port" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (listen(master_socket, 3) < 0) {
        std::cout << "listen System Call Error" << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "Server on Port: " << PORT << std::endl;

    std::cout << "Waiting for connections...." << std::endl;

    std::thread(handleInput).detach();

    while (true) {
        addrlen = sizeof(address);
        new_socket = accept(master_socket, (sockaddr *)&address, (socklen_t *)&addrlen);

        if (new_socket == -1) {
            continue;
        }

        std::string conMsg1 = (std::string) "\x1B[32m" + "Connected to client " + std::to_string(new_socket) + (std::string) "\x1b[0m";
        const char *conMsg = conMsg1.data();

        for (auto x : clientSockets) {
            std::string to_new_str = (std::string) "\x1B[32m" + "Connected to client " + std::to_string(x.first) + (std::string) "\x1b[0m";
            const char *to_new = to_new_str.data();
            send(new_socket, to_new, strlen(to_new), 0);
            send(x.first, conMsg, strlen(conMsg), 0);
            sleep(1);
        }

        clientSockets[new_socket] = new_socket;

        // Create a new thread to handle the client
        std::thread clientThread(handleClient, new_socket);
        clientThread.detach();
    }

    close(master_socket);
    for (auto x : clientSockets) {
        close(x.first);
    }
    clientSockets.clear();

    return 0;
}
