cmake_minimum_required(VERSION 3.15)

project(shared_memory_demo)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

find_package(Boost
  1.82.0
  REQUIRED
)

include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} )

include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/generated)

# Set the sources for the shared_memory_demo executable
set(SHARED_MEMORY_DEMO_SOURCES
    src/main.cpp
    generated/message_generated.h
)

# Create the shared_memory_demo executable
add_executable(shared_memory_demo ${SHARED_MEMORY_DEMO_SOURCES})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(shared_memory_demo ${Boost_LIBRARIES})
