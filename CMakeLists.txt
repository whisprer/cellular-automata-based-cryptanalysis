cmake_minimum_required(VERSION 3.20)
project(caca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Gather all source files recursively from src/
file(GLOB_RECURSE SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

# If you don’t have a src/ folder, use this instead:
# file(GLOB_RECURSE SRC_FILES ${PROJECT_SOURCE_DIR}/*.cpp)

# Add the executable target
add_executable(caca_app ${SRC_FILES})

# Optional: include directories if needed
# target_include_directories(caca_app PRIVATE ${PROJECT_SOURCE_DIR}/include)
