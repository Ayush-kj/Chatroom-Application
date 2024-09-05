# Chatroom Application

## Overview
The **Chatroom Application** is a multi-client chat system developed in C++ using **socket programming** and **multi-threading**. The server supports multiple clients connecting simultaneously, allowing them to exchange messages in real-time. The system is built using **POSIX threads** and **TCP sockets**, and it supports both broadcast messaging and direct messaging between clients.

### Key Features:
- **Multi-client Support:** Multiple clients can connect to the server and communicate concurrently.
- **Broadcast Messaging:** Clients can send messages to all connected clients.
- **Private Messaging:** Clients can also send direct messages to specific clients using their unique client ID.
- **Concurrency Control:** The server uses multi-threading to handle multiple client connections simultaneously.
- **Server Shutdown:** A server shutdown mechanism is included to gracefully disconnect clients when the server is stopped.
- **Error Handling:** Basic error handling ensures smooth communication and disconnection.

---

## Technologies Used
- **C++**: Primary language for the application.
- **Socket Programming**: Used for TCP connections between the server and clients.
- **POSIX Threads**: Used to manage concurrency, allowing multiple clients to communicate with the server simultaneously.
- **Networking**: TCP/IP protocols for real-time communication.

---

## Directory Structure
```
/ChatroomApplication
    ├── server.cpp        # The server-side code
    ├── client.cpp        # The client-side code
    └── README.md         # This documentation
```

---

## How to Run the Application

### Prerequisites
To compile and run this project, you will need:
- A Linux or macOS environment (due to POSIX socket usage)
- A C++ compiler (g++, clang++, etc.)

### Step 1: Compile the Server and Client

1. **Navigate to the project directory**:
    ```bash
    cd /path/to/ChatroomApplication
    ```

2. **Compile the server**:
    ```bash
    g++ -std=c++11 server.cpp -o server -pthread
    ```

3. **Compile the client**:
    ```bash
    g++ -std=c++11 client.cpp -o client -pthread
    ```

### Step 2: Run the Server

1. **Start the server** by specifying the port number:
    ```bash
    ./server <port_number>
    ```
   Replace `<port_number>` with your desired port number, e.g.:
    ```bash
    ./server 8080
    ```

2. The server will now be running and waiting for client connections on the specified port.

### Step 3: Run the Clients

1. **Start a client** by specifying the server IP and port number:
    ```bash
    ./client <server_ip> <port_number>
    ```
   Example:
    ```bash
    ./client 127.0.0.1 8080
    ```

2. You can open multiple terminals and repeat this step to connect multiple clients to the server.

---

## Usage Instructions

### Server Commands
- **/exit**: Shut down the server and disconnect all clients gracefully.

### Client Commands
- **<client_number> <message>**: Send a message to a specific client. The `client_number` is the unique ID assigned to each client upon connection.
- **1 <message>**: Send a broadcast message to all connected clients.
- **/exit**: Disconnect the client from the server.

---

## Example Walkthrough

### Step 1: Start the Server
```bash
./server 8080
```
Output:
```
Server on Port: 8080
Waiting for connections...
```

### Step 2: Start Multiple Clients
```bash
./client 127.0.0.1 8080
```

Output on Client 1:
```
Connected to the server
Client ID: 4
```

### Step 3: Send Messages
- **Client 1** sends a broadcast message:
    ```
    1 Hello everyone!
    ```
- **Client 2** receives the message:
    ```
    Client 4: Hello everyone!
    ```

### Step 4: Server Shutdown
From the server console, type:
```bash
/exit
```
This will disconnect all clients and shut down the server.

---

## Error Handling
- If a client sends an incorrect message format (e.g., omitting the client number or message), they will receive the following error:
    ```
    Please use the correct format: <clientNo.> <Msg>
    ```
- If a client disconnects or the server shuts down, remaining clients will be notified with a disconnection message:
    ```
    Client X disconnected
    ```

---

## Future Improvements
- **GUI Implementation:** Add a graphical interface for ease of use.
- **Encrypted Messaging:** Implement end-to-end encryption to secure the communication.
- **Enhanced Error Handling:** Improve error handling for connection failures, message loss, and re-connection attempts.

---

## Conclusion
This **Chatroom Application** is a basic implementation of a real-time messaging system using **C++**, **socket programming**, and **multi-threading**. It can be extended with additional features such as encryption, user authentication, and more advanced messaging functionalities.
```

### Instructions to Use:
- Copy this code and save it in a file named `README.md` in your project directory.
- Push the changes to your GitHub repository, and this file will appear as the documentation for your project.

This README provides detailed information about the project and how to run it, making it easy for others to understand and use your chatroom application.
