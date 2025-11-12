#include "epoll_wrapper.hpp"
#include <stdexcept>
#include <cstring>

EpollWrapper::EpollWrapper(int max_events)
    : max_events_(max_events)
{
    epoll_fd_ = epoll_create1(0);
    if (epoll_fd_ == -1) {
        throw std::runtime_error("Failed to create epoll instance");
    }
}

EpollWrapper::~EpollWrapper() {
    close(epoll_fd_);
}

void EpollWrapper::add_fd(int fd, uint32_t events) {
    epoll_event ev{};
    ev.events = events;
    ev.data.fd = fd;
    if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("Failed to add fd to epoll");
    }
}

void EpollWrapper::remove_fd(int fd) {
    epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, fd, nullptr);
}

std::vector<epoll_event> EpollWrapper::wait(int timeout_ms) {
    std::vector<epoll_event> events(max_events_);
    int n = epoll_wait(epoll_fd_, events.data(), max_events_, timeout_ms);
    if (n < 0) {
        if (errno == EINTR) return {}; // interrupt -> ignore
        throw std::runtime_error("epoll_wait failed");
    }
    events.resize(n);
    return events;
}