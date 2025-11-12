#include "server.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <cstring>

namespace {
    void set_nonblocking(int fd) {
        int flags = fcntl(fd, F_GETFL, 0);
        fcntl(fd, F_SETFL, flags | O_NONBLOCK);
    }
}

TCPServer::TCPServer(int port)
    : port_(port), listen_fd_(-1), epoll_(128)
{
    setup_server_socket();
}

TCPServer::~TCPServer() {
    if (listen_fd_ != -1) close(listen_fd_);
}

void TCPServer::setup_server_socket() {
    listen_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd_ < 0) throw std::runtime_error("socket() failed");

    int opt = 1;
    setsockopt(listen_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listen_fd_, (sockaddr*)&addr, sizeof(addr)) < 0)
        throw std::runtime_error("bind() failed");

    if (listen(listen_fd_, SOMAXCONN) < 0)
        throw std::runtime_error("listen() failed");

    set_nonblocking(listen_fd_);
    epoll_.add_fd(listen_fd_, EPOLLIN);
}

void TCPServer::run() {
    std::cout << "Server listening on port " << port_ << "...\n";

    while (true) {
        auto events = epoll_.wait(1000);

        for (auto &ev : events) {
            int fd = ev.data.fd;

            if (fd == listen_fd_) {
                handle_new_connection();
            } else if (ev.events & EPOLLIN) {
                handle_client_event(fd);
            }
        }
    }
}

void TCPServer::handle_new_connection() {
    sockaddr_in client_addr{};
    socklen_t len = sizeof(client_addr);
    int client_fd = accept(listen_fd_, (sockaddr*)&client_addr, &len);
    if (client_fd < 0) return;

    set_nonblocking(client_fd);
    epoll_.add_fd(client_fd, EPOLLIN);

    std::cout << "[+] New client connected: " 
              << inet_ntoa(client_addr.sin_addr) << "\n";
}

void TCPServer::handle_client_event(int client_fd) {
    char buffer[1024];
    ssize_t bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);

    if (bytes <= 0) {
        std::cout << "[-] Client disconnected: " << client_fd << "\n";
        epoll_.remove_fd(client_fd);
        close(client_fd);
        return;
    }

    buffer[bytes] = '\0';
    std::string response = "Echo: " + std::string(buffer);
    send(client_fd, response.c_str(), response.size(), 0);
}
