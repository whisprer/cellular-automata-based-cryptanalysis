cmake_minimum_required(VERSION 3.20)
project(caca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all source files recursively
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# Build the executable
add_executable(caca_app ${SRC_FILES})

# Optional include dirs
# target_include_directories(caca_app PRIVATE ${PROJECT_SOURCE_DIR}/include)
