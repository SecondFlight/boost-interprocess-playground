#include <boost/interprocess/ipc/message_queue.hpp>
#include <iostream>
#include <vector>

using namespace boost::interprocess;

int main ()
{
    try {
        // Create a message_queue.
        message_queue mqFromClient(
            open_only,                 // only create
            "client-to-server"         // name
        );

        // Open a message_queue.
        message_queue mqToClient(
            create_only,               // only create
            "server-to-client",        // name
            100,                       // max message number
            sizeof(int)                // max message size
        );

        unsigned int priority;
        message_queue::size_type recvd_size;

        int counter = 0;

        // Receive 100 numbers
        for (int i = 0; i < 100; ++i) {
            int number;
            mqFromClient.receive(&number, sizeof(number), recvd_size, priority);
            if (number != i || recvd_size != sizeof(number))
                return 1;
            counter += number;
        }

        mqToClient.send(&counter, sizeof(counter), 0);
    }
    catch(interprocess_exception &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
