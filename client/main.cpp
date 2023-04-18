#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/process.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;
namespace bp = boost::process;

int main(int argc, char* argv[])
{
    // Get the directory where the executable is located
    std::string executable_path = bp::search_path(argv[0]).parent_path().string();

    std::string server_path = executable_path + "/../../../server/build/Debug/server.exe"; // Default value

    if (argc >= 2) {
        server_path = executable_path + "/" + argv[1]; // Use user-specified path
    }

    std::cout << "client: Client start." << std::endl;

    std::cout << "client: Starting server..." << std::endl;

    // Start the server process
    bp::child server(server_path);

    try {
        std::cout << "client: Removing old message queue..." << std::endl;

        // Erase previous message queue
        try {
            message_queue::remove("client-to-server");
            message_queue::remove("server-to-client");
        } catch (...) {}

        std::cout << "client: Message queue removed. Creating new message queue..." << std::endl;

        // Create a message_queue.
        message_queue mqToServer(
            create_only,               // only create
            "client-to-server",        // name
            100,                       // max message number
            sizeof(int)                // max message size
        );

        std::cout << "client: Finding other message queue..." << std::endl;

        message_queue* mqFromServer;

        while(true) {
            try {
                // Open a message_queue.
                mqFromServer = new message_queue(
                    open_only,                 // only create
                    "server-to-client"         // name
                );
                break;
                boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
            } catch(...) {}
        }

        std::cout << "client: Done. Sending..." << std::endl;

        // Send 100 numbers
        for (int i = 0; i < 100; ++i) {
            mqToServer.send(&i, sizeof(i), 0);
        }

        int result;
        message_queue::size_type recvd_size;
        unsigned int priority;

        mqFromServer->receive(&result, sizeof(result), recvd_size, priority);
        
        std::cout << "Result: " << result << std::endl;
    }
    catch(interprocess_exception &ex) {
        message_queue::remove("client-to-server");
        message_queue::remove("server-to-client");

        std::cout << ex.what() << std::endl;
        return 1;
    }
    
    message_queue::remove("client-to-server");
    message_queue::remove("server-to-client");
    return 0;
}
