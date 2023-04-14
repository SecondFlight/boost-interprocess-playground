#include <boost/interprocess/ipc/message_queue.hpp>
#include "../shared/messages.hpp"
#include <iostream>

namespace ipc = boost::interprocess;

int main() {
    // Wait for connection and messages
    ipc::message_queue client_queue(ipc::open_or_create, "client_queue", 100, sizeof(Message));
    ipc::message_queue server_queue(ipc::create_only, "server_queue", 100, sizeof(Message));

    // Receive a message from the client
    Message msg_received;
    unsigned int priority;
    ipc::message_queue::size_type recvd_size;

    client_queue.receive(&msg_received, sizeof(Message), recvd_size, priority);

    // Send a message back to the client
    Message msg_to_send{2, "Hello, client!"};
    server_queue.send(&msg_to_send, sizeof(Message), 0);

    // Clean up
    ipc::message_queue::remove("client_queue");
    ipc::message_queue::remove("server_queue");
    return 0;
}
