#include <string>
#include <cstdlib>
#include <iostream>

// Declaration of run_server from server.cpp
int run_server(int port, const std::string& host);

int main(int argc, char* argv[]) {
    int port = 8080;
    std::string host = "0.0.0.0";

    // Read from env var if present
    if (const char* env_p = std::getenv("PORT")) {
        port = std::stoi(env_p);
    }
    if (const char* env_h = std::getenv("HOST")) {
        host = env_h;
    }

    // Read from command-line arguments if present
    if (argc > 1) {
        port = std::stoi(argv[1]);
    }
    if (argc > 2) {
        host = argv[2];
    }

    return run_server(port, host);
}
