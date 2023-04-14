cmake_minimum_required(VERSION 3.15)

project(client)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT "C:/Users/joshwade/Documents/Code/boost_1_82_0")

find_package(Boost
  1.82.0
  REQUIRED
)

# Set the sources for the client executable
set(CLIENT_SOURCES
    main.cpp
)

# Create the client executable
add_executable(client ${CLIENT_SOURCES})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(client ${Boost_LIBRARIES})
