cmake_minimum_required(VERSION 3.15)

project(server)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

set(BOOST_ROOT "C:/Users/joshwade/Documents/Code/boost_1_82_0")

find_package(Boost
  1.82.0
  REQUIRED
)

# Set the sources for the server executable
set(SERVER_SOURCES
    main.cpp
)

# Create the server executable
add_executable(server ${SERVER_SOURCES})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(server ${Boost_LIBRARIES})
