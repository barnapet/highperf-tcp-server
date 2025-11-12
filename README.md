# 🧩 High-Performance TCP Server (C++17 / epoll / Linux)

A minimal, high-performance TCP echo server built with C++17 and epoll.
Designed as a foundation for scalable, low-latency network applications.

## Features
- Epoll-based I/O multiplexing
- Non-blocking sockets
- RAII-style epoll wrapper
- Clean CMake build system

## Build & Run
```bash
mkdir build && cd build
cmake ..
make
./tcp_server
```

## Next steps
- Thread pool for concurrency
- Benchmarking with wrk/netcat
- Profiling with Valgrind and perf
