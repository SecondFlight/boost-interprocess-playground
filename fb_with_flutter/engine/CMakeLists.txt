cmake_minimum_required(VERSION 3.15)

project(engine_demo)

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

# Include the cpp_shared directory
include_directories(${CMAKE_SOURCE_DIR}/../cpp_shared)

# Set the sources for the engine_demo executable
set(ENGINE_DEMO_SOURCES
    src/main.cpp
    generated/messages_generated.h
    ../cpp_shared/create_message_queue.h
    ../cpp_shared/create_message_queue.cpp
)

# Create the engine_demo executable
add_executable(engine_demo ${ENGINE_DEMO_SOURCES})

target_link_libraries(engine_demo ${Boost_LIBRARIES})
