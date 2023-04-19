#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <string>

#include "message_generated.h"

void client() {
    std::cout << "Calling client() function." << std::endl;
}

void server() {
    std::cout << "Calling server() function." << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " client|server" << std::endl;
        return 1;
    }

    std::string argument(argv[1]);

    if (argument == "client") {
        client();
    } else if (argument == "server") {
        server();
    } else {
        std::cerr << "Invalid argument: " << argument << std::endl;
        return 1;
    }

    return 0;
}
