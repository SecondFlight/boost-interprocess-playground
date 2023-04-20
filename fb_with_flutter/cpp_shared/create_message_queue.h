#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/thread.hpp>

using namespace boost::interprocess;

std::unique_ptr<message_queue> openMessageQueue(const char *name);
