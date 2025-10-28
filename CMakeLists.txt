cmake_minimum_required(VERSION 3.20)
project(caca LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Detect AVX2 support
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2)
    add_compile_options(-mavx2)
    message(STATUS "AVX2 enabled for build")
else()
    message(WARNING "AVX2 not supported on this system, compiling scalar fallback")
    add_definitions(-DNO_AVX)
endif()

# Find all .cpp files in this directory and below
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_SOURCE_DIR}/*.cpp"
)

if(NOT SRC_FILES)
    message(FATAL_ERROR "No source files found! Check your project structure.")
endif()

add_executable(caca_app ${SRC_FILES})
