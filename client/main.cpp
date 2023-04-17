#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/process.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;
namespace bp = boost::process;

int main()
{
    std::cout << "client: Client start." << std::endl;

    std::cout << "client: Starting server..." << std::endl;

    // Start the server process
    bp::child server("./server.exe");

    try {
        std::cout << "client: Removing old message queue..." << std::endl;

        // Erase previous message queue
        message_queue::remove("message_queue");

        std::cout << "client: Message queue removed. Creating new message queue..." << std::endl;

        // Create a message_queue.
        message_queue mq(
            create_only,               // only create
            "message_queue",           // name
            100,                       // max message number
            sizeof(int)                // max message size
        );

        std::cout << "client: Done. Sending..." << std::endl;

        // Send 100 numbers
        for (int i = 0; i < 100; ++i) {
            mq.send(&i, sizeof(i), 0);
        }
    }
    catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
