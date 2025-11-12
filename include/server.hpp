#pragma once
#include <string>
#include <vector>
#include "epoll_wrapper.hpp"

class TCPServer {
public:
    TCPServer(int port);
    ~TCPServer();

    void run();

private:
    int port_;
    int listen_fd_;
    EpollWrapper epoll_;

    void setup_server_socket();
    void handle_new_connection();
    void handle_client_event(int client_fd);
};