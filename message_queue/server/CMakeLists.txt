cmake_minimum_required(VERSION 3.15)

project(server)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

find_package(Boost
  1.82.0
  REQUIRED
)

include_directories( ${Boost_INCLUDE_DIR} )
link_directories( ${Boost_LIBRARY_DIRS} )

# Set the sources for the server executable
set(SERVER_SOURCES
    main.cpp
)

# Create the server executable
add_executable(server ${SERVER_SOURCES})

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(server ${Boost_LIBRARIES})
