#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    // Create socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket failed");
        return 1;
    }

    // Define address structure
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;  // Listen on all interfaces
    address.sin_port = htons(8080);        // Port 8080

    // Bind socket to port
    if (bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return 1;
    }

    // Start listening
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        return 1;
    }

    std::cout << "Server listening on port 8080...\n";

    while (true) {
        // Accept client connection
        int client_fd = accept(server_fd, nullptr, nullptr);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }

        // Read request
        char buffer[4096] = {0};
        read(client_fd, buffer, sizeof(buffer));

        std::cout << "Request received:\n" << buffer << "\n";

        // Create HTTP response
        const char* response =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/plain\r\n"
            "Content-Length: 13\r\n"
            "\r\n"
            "Hello, world!";

        // Send response
        send(client_fd, response, strlen(response), 0);

        // Close connection
        close(client_fd);
    }

    close(server_fd);
    return 0;
}