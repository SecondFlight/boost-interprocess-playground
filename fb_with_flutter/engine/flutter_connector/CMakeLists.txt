cmake_minimum_required(VERSION 3.15)

project(FlutterEngineConnector VERSION 1.0.0 LANGUAGES CXX)

# Set the C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

find_package(Boost 1.82.0 REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

# Set the sources for the FlutterEngineConnector executable
set(FLUTTER_ENGINE_CONNECTOR_SOURCES
    src/main.cpp
)

# Create the FlutterEngineConnector shared library (DLL)
add_library(FlutterEngineConnector SHARED ${FLUTTER_ENGINE_CONNECTOR_SOURCES})

# Define the export symbol
target_compile_definitions(FlutterEngineConnector PRIVATE FLUTTER_ENGINE_CONNECTOR_EXPORTS)

# Link with the Boost libraries
target_link_libraries(FlutterEngineConnector ${Boost_LIBRARIES})
