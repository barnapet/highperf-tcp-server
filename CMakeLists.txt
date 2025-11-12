cmake_minimum_required(VERSION 3.10)
project(HighPerfTCPServer CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SERVER_SOURCES
    src/main.cpp
    src/server.cpp
    src/epoll_wrapper.cpp
)

add_executable(tcp_server ${SERVER_SOURCES})

target_compile_options(tcp_server PRIVATE
    -Wall
    -Wextra
    $<$<CONFIG:Release>:-O2>
    $<$<CONFIG:Debug>:-O0>
)

target_link_libraries(tcp_server PRIVATE pthread)

target_include_directories(tcp_server PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
