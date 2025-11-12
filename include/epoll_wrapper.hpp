#pragma once
#include <sys/epoll.h>
#include <vector>
#include <stdexcept>
#include <unistd.h>

class EpollWrapper {
public:
    explicit EpollWrapper(int max_events = 64);
    ~EpollWrapper();

    void add_fd(int fd, uint32_t events);
    void remove_fd(int fd);
    std::vector<epoll_event> wait(int timeout_ms);

private:
    int epoll_fd_;
    int max_events_;
};