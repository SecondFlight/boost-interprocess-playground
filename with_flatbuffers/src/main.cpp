#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <string>

#include "message_generated.h"

using namespace boost::interprocess;

std::unique_ptr<message_queue> openMessageQueue(const char *name)
{
    int count = 0;

    while (true)
    {
        count++;

        try
        {
            // Open a message_queue.
            return std::unique_ptr<message_queue>(
                new message_queue(
                    open_only,
                    name));
        }
        catch (...)
        {
            boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        }

        if (count > 100)
        {
            throw std::runtime_error("Failed to open message queue.");
        }
    }

    message_queue::remove("server-to-client");
}

void client()
{
    std::cout << "Cleanup, just in case" << std::endl;

    message_queue::remove("client-to-server");

    std::cout << "Creating client-to-server message queue" << std::endl;

    // Create a message_queue.
    auto mqToServer = std::unique_ptr<message_queue>(
        new message_queue(
            create_only,
            "client-to-server",

            // 100 messages can be in the queue at once
            100,

            // Each message can be a maximum of 65536 bytes (?) long
            65536));

    std::cout << "Opening server-to-client message queue" << std::endl;
    auto mqFromServer = openMessageQueue("server-to-client");

    message_queue::remove("client-to-server");
}

void server()
{
    std::cout << "Cleanup, just in case" << std::endl;

    message_queue::remove("server-to-client");

    std::cout << "Creating server-to-client message queue" << std::endl;

    // Create a message_queue.
    auto mqToClient = std::unique_ptr<message_queue>(
        new message_queue(
            create_only,
            "server-to-client",

            // 100 messages can be in the queue at once
            100,

            // Each message can be a maximum of 65536 bytes (?) long
            65536));

    std::cout << "Opening client-to-server message queue" << std::endl;
    auto mqFromClient = openMessageQueue("client-to-server");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " client|server" << std::endl;
        return 1;
    }

    std::string argument(argv[1]);

    if (argument == "client")
    {
        client();
    }
    else if (argument == "server")
    {
        server();
    }
    else
    {
        std::cerr << "Invalid argument: " << argument << std::endl;
        return 1;
    }

    return 0;
}