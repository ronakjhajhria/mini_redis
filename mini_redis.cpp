#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

// ============================================================================
// MINI REDIS - DAY 1-3: ECHO SERVER
// ============================================================================

void handle_client(int client_socket) {
    // Buffer to store received data
    char buffer[1024];

    // Read data from client
    int bytes_received = read(client_socket, buffer, sizeof(buffer));

    if (bytes_received > 0) {
        // Print received message
        cout << "Received: "
             << string(buffer, bytes_received)
             << endl;

        // Echo back to client
        write(client_socket, buffer, bytes_received);
    }

    // Close client socket
    close(client_socket);
}

int main() {
    cout << "Starting Mini Redis Echo Server...\n";

    // Create TCP socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0) {
        cerr << "Socket creation failed\n";
        return 1;
    }

    // Create server address
    sockaddr_in server_addr;

    // Clear the structure
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(6379);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket to port
    if (bind(server_socket,
             (struct sockaddr*)&server_addr,
             sizeof(server_addr)) < 0) {
        cerr << "Bind failed\n";
        return 1;
    }

    cout << "Socket bound to port 6379\n";

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) {
        cerr << "Listen failed\n";
        return 1;
    }

    cout << "Listening for connections...\n";

    while (true) {

        // Wait for a client
        int client_socket = accept(server_socket, NULL, NULL);

        if (client_socket < 0) {
            cerr << "Accept failed\n";
            continue;
        }

        cout << "Client connected!\n";

        // Handle each client in a separate thread
        thread(handle_client, client_socket).detach();
    }

    close(server_socket);

    return 0;
}
