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

int client()
{
    std::cout << "Cleanup, just in case" << std::endl;

    message_queue::remove("client-to-server");
    message_queue::remove("server-to-client");

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

    // Create a buffer for receiving messages from the server process
    uint8_t buffer[65536];

    int request_id = 0;

    while (true) {
        // Get command string
        std::cout << "Enter a command (exit, add, subtract): ";
        std::string command;
        std::cin >> command;

        // Create flatbuffers builder
        auto builder = flatbuffers::FlatBufferBuilder();

        int a, b;
        Command request_command_type;
        flatbuffers::Offset<void> command_offset;

        bool exit = false;
        
        if (command == "exit") {
            exit = true;
            auto exit = CreateExit(builder);
            command_offset = exit.Union();
            request_command_type = Command_Exit;
        } else if (command == "add") {
            std::cout << "Enter A: ";
            std::cin >> a;
            std::cout << "Enter B: ";
            std::cin >> b;
            auto add = CreateAdd(builder, a, b);
            command_offset = add.Union();
            request_command_type = Command_Add;
        } else if (command == "subtract") {
            std::cout << "Enter A: ";
            std::cin >> a;
            std::cout << "Enter B: ";
            std::cin >> b;
            auto subtract = CreateSubtract(builder, a, b);
            command_offset = subtract.Union();
            request_command_type = Command_Subtract;
        } else {
            std::cout << "Invalid command. Try again." << std::endl;
            continue;
        }

        // Create the request object
        auto request = CreateRequest(builder, request_id, request_command_type, command_offset);

        builder.Finish(request);

        auto send_buffer_ptr = builder.GetBufferPointer();
        auto buffer_size = builder.GetSize();

        // Send the request to the server
        mqToServer->send(send_buffer_ptr, buffer_size, 0);

        // If we want to exit, we don't wait for a response since the server won't send one.
        if (exit) return 0;

        request_id++;

        std::size_t received_size;
        unsigned int priority;

        // Get the server's response
        mqFromServer->receive(buffer, sizeof(buffer), received_size, priority);

        // Create a const pointer to the start of the buffer
        const void* receive_buffer_ptr = static_cast<const void*>(buffer);

        // Get the root of the buffer
        auto response = flatbuffers::GetRoot<Response>(receive_buffer_ptr);

        int request_id = response->id();
        auto response_command_type = response->return_value_type();

        switch (response_command_type) {
            case ReturnValue_AddReturnValue: {
                auto return_value = response->return_value_as_AddReturnValue();
                std::cout << "Got response: " << return_value->value() << std::endl;
                break;
            }
            case ReturnValue_SubtractReturnValue: {
                auto return_value = response->return_value_as_SubtractReturnValue();
                std::cout << "Got response: " << return_value->value() << std::endl;
                break;
            }
        }
    }

    return 0;
}

int server()
{
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

    uint8_t buffer[65536];

    while (true) {
        std::size_t received_size;
        unsigned int priority;

        mqFromClient->receive(buffer, sizeof(buffer), received_size, priority);

        // Create a const pointer to the start of the buffer
        const void* send_buffer_ptr = static_cast<const void*>(buffer);

        // Get the root of the buffer
        auto request = flatbuffers::GetRoot<Request>(send_buffer_ptr);

        // Create flatbuffers builder
        auto builder = flatbuffers::FlatBufferBuilder();

        flatbuffers::Offset<void> return_value_offset;

        // Access the data in the buffer
        int request_id = request->id();
        auto command_type = request->command_type();
        int a, b;
        ReturnValue return_value_type;
        switch (command_type) {
            case Command_Add: {
                auto add = request->command_as_Add();
                a = add->a();
                b = add->b();
                std::cout << "Received Add command: " << a << " + " << b << std::endl;
                auto add_return_value = CreateAddReturnValue(builder, a + b);
                return_value_offset = add_return_value.Union();
                return_value_type = ReturnValue_AddReturnValue;
                break;
            }
            case Command_Subtract: {
                auto subtract = request->command_as_Subtract();
                a = subtract->a();
                b = subtract->b();
                std::cout << "Received Subtract command: " << a << " - " << b << std::endl;
                auto subtract_return_value = CreateSubtractReturnValue(builder, a - b);
                return_value_offset = subtract_return_value.Union();
                return_value_type = ReturnValue_SubtractReturnValue;
                break;
            }
            case Command_Exit: {
                return 0;
            }
            // Handle other commands here
            default: {
                std::cerr << "Received unknown command" << std::endl;
                break;
            }
        }

        // Create the request object
        auto response = CreateResponse(builder, request_id, return_value_type, return_value_offset);

        builder.Finish(response);

        auto receive_buffer_ptr = builder.GetBufferPointer();
        auto buffer_size = builder.GetSize();

        // Send the request to the server
        mqToClient->send(receive_buffer_ptr, buffer_size, 0);
    }

    return 0;
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
        return client();
    }
    else if (argument == "server")
    {
        return server();
    }
    else
    {
        std::cerr << "Invalid argument: " << argument << std::endl;
        return 1;
    }
}