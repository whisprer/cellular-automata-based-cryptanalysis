cmake_minimum_required(VERSION 3.20)
project(caca)

set(CMAKE_CXX_STANDARD 17)

# Gather all source files
file(GLOB_RECURSE SRC_FILES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    # or if your code is in . instead of src, adjust accordingly
)

# Example: build an executable
add_executable(caca_app ${SRC_FILES})
