#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>

#include "create_message_queue.h"

using namespace boost::interprocess;

std::unique_ptr<boost::interprocess::message_queue> mqToEngine;
std::unique_ptr<boost::interprocess::message_queue> mqFromEngine;

// Buffer for receiving messages from the server process
uint8_t *message_receive_buffer = new uint8_t[65536];

// Buffer for sending messages to the server process
uint8_t *message_send_buffer = new uint8_t[65536];
std::size_t last_received_size;
unsigned int last_received_priority;

int request_id = 0;

extern "C"
{
    _declspec(dllexport) void _stdcall cleanupPreviousMessageQueues()
    {
        std::cout << "Cleanup, just in case" << std::endl;

        message_queue::remove("ui-to-engine");
        message_queue::remove("engine-to-ui");
    }

    __declspec(dllexport) void __stdcall connect()
    {
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
        std::cout << "Opened successfully" << std::endl;
    }

    __declspec(dllexport) uint8_t *__stdcall getMessageSendBuffer()
    {
        return message_send_buffer;
    }

    __declspec(dllexport) uint8_t *__stdcall getMessageReceiveBuffer()
    {
        return message_receive_buffer;
    }

    __declspec(dllexport) std::size_t __stdcall getLastReceivedMessageSize()
    {
        return last_received_size;
    }

    __declspec(dllexport) void __stdcall sendFromBuffer(int64_t size)
    {
        mqToEngine->send(message_send_buffer, size, 0);
    }

    // This blocks the current thread until a message is received.
    _declspec(dllexport) bool __stdcall receive()
    {
        try {
            mqFromEngine->receive(message_receive_buffer, 65536, last_received_size, last_received_priority);
            return true;
        }
        catch (const interprocess_exception &ex) {
            std::cerr << "Error receiving message: " << ex.what() << std::endl;
            return false;
        }
        catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return false;
        }
    }

    _declspec(dllexport) bool __stdcall tryReceive()
    {
        try {
            return mqFromEngine->try_receive(message_receive_buffer, 65536, last_received_size, last_received_priority);
        }
        catch (const interprocess_exception &ex) {
            std::cerr << "Error receiving message: " << ex.what() << std::endl;
            return false;
        }
        catch (const std::exception &ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return false;
        }
    }
}
