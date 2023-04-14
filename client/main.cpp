#include <iostream>
#include <boost/interprocess/managed_shared_memory.hpp>

int main() {
    using namespace boost::interprocess;

    // Create a managed shared memory segment
    managed_shared_memory segment(create_only, "MySharedMemory", 65536);

    // Allocate a portion of the shared memory
    const int num_elements = 100;
    const int element_size = sizeof(int);
    void* allocated_memory = segment.allocate(num_elements * element_size);

    // Deallocate the memory
    segment.deallocate(allocated_memory);

    // Remove the shared memory segment
    shared_memory_object::remove("MySharedMemory");

    std::cout << "Boost.Interprocess example completed." << std::endl;

    return 0;
}
