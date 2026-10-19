#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>
#include <iostream>

int main() {
    // 1. Create a TCP socket (a "file descriptor" = a number the OS gives us)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); return 1; }

    // Allow restarting the server immediately on the same port
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 2. Say which address and port we want
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);  // accept connections on any network interface
    addr.sin_port = htons(6380);               // port number (htons = convert to network byte order)

    // 3. Attach the socket to that address, then start listening
    if (bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) { perror("bind"); return 1; }
    if (listen(server_fd, 16) < 0) { perror("listen"); return 1; }
    std::cout << "Listening on port 6380\n";

    // 4. Serve one client at a time (we will fix this limitation in Step 4)
    while (true) {
        int client_fd = accept(server_fd, nullptr, nullptr);  // waits here until a client connects
        if (client_fd < 0) { perror("accept"); continue; }
        std::cout << "Client connected\n";

        char buf[1024];
        while (true) {
            ssize_t n = read(client_fd, buf, sizeof(buf));
            if (n <= 0) break;       // 0 = client closed the connection, negative = error
            write(client_fd, buf, n); // send the same bytes back
        }
        close(client_fd);
        std::cout << "Client disconnected\n";
    }
}