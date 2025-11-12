#include "server.hpp"
#include <iostream>

int main() {
    try {
        TCPServer server(8080);
        server.run();
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}