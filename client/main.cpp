#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/process.hpp>
#include "../shared/messages.hpp"
#include <iostream>

namespace ipc = boost::interprocess;
namespace bp = boost::process;

int main() {
    std::cout << "1";
    // Start the server process
    bp::child server("./server.exe");
    std::cout << "2";

    // Set up a message queue with the server
    ipc::message_queue::remove("client_queue");
    ipc::message_queue::remove("server_queue");

    std::cout << "3";

    ipc::message_queue client_queue(ipc::create_only, "client_queue", 100, sizeof(Message));
    ipc::message_queue server_queue(ipc::open_or_create, "server_queue", 100, sizeof(Message));

    std::cout << "4";

    // Send a message from a class
    Message msg_to_send{1, "Hello, server!"};
    client_queue.send(&msg_to_send, sizeof(Message), 0);

    std::cout << "5";

    // Receive a message from the server and log it to standard out
    Message msg_received;
    unsigned int priority;
    ipc::message_queue::size_type recvd_size;

    std::cout << "6";

    server_queue.receive(&msg_received, sizeof(Message), recvd_size, priority);
    std::cout << "Received message: " << msg_received.content << std::endl;

    std::cout << "7";

    // Clean up
    ipc::message_queue::remove("client_queue");
    ipc::message_queue::remove("server_queue");
    server.terminate();
    return 0;
}
