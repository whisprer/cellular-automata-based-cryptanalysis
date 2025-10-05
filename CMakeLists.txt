cmake_minimum_required(VERSION 3.20)
project(caca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adjust this path depending on where your .cpp files actually live:
file(GLOB_RECURSE SRC_FILES
    ${CMAKE_SOURCE_DIR}/v-5-1-4/*.cpp
)

if(NOT SRC_FILES)
    message(FATAL_ERROR "No source files found! Check your file paths.")
endif()

add_executable(caca_app ${SRC_FILES})
