#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

#include "../../cpp_shared/create_message_queue.h"

using namespace boost::interprocess;

std::unique_ptr<boost::interprocess::message_queue> mqToEngine;
std::unique_ptr<boost::interprocess::message_queue> mqFromEngine;

// Buffer for receiving messages from the server process
uint8_t* message_receive_buffer = new uint8_t[65536];

// Buffer for sending messages to the server process
uint8_t* message_send_buffer = new uint8_t[65536];

int request_id = 0;

extern "C"
{
    __declspec(dllexport) void __stdcall connect()
    {
        std::cout << "Cleanup, just in case" << std::endl;

        message_queue::remove("ui-to-engine");
        message_queue::remove("engine-to-ui");

        std::cout << "Creating ui-to-engine message queue" << std::endl;

        // Create a message_queue.
        mqToEngine = std::unique_ptr<message_queue>(
            new message_queue(
                create_only,
                "ui-to-engine",

                // 100 messages can be in the queue at once
                100,

                // Each message can be a maximum of 65536 bytes (?) long
                65536));

        std::cout << "Opening engine-to-ui message queue" << std::endl;
        mqFromEngine = openMessageQueue("engine-to-ui");
    }

    __declspec(dllexport) uint8_t* __stdcall getMessageBuffer()
    {
        return message_send_buffer;
    }

    __declspec(dllexport) void __stdcall sendFromBuffer(int64_t size) {
        mqToEngine->send(message_send_buffer, size, 0);
    }
}
