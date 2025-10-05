cmake_minimum_required(VERSION 3.20)
project(caca)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable AVX2 if supported
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
if(COMPILER_SUPPORTS_AVX2)
    add_compile_options(-mavx2)
    message(STATUS "AVX2 enabled for build")
else()
    message(WARNING "AVX2 not supported on this system, building scalar fallback only")
    add_definitions(-DNO_AVX)
endif()

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(caca_app ${SRC_FILES})
