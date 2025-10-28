cmake_minimum_required(VERSION 3.20)
project(CACA VERSION 1.0
    DESCRIPTION "Cellular Automata CryptAnalysis"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler-specific flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    
    # Enable position-independent code
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    
    # Enable SIMD optimizations
    if(ENABLE_SIMD)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("-mavx" AVX_SUPPORTED)
        check_cxx_compiler_flag("-mavx2" AVX2_SUPPORTED)
        check_cxx_compiler_flag("-mavx512f" AVX512F_SUPPORTED)
        
        if(AVX512F_SUPPORTED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512f")
        elseif(AVX2_SUPPORTED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
        elseif(AVX_SUPPORTED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
        endif()
    endif()
elseif(MSVC)
    # MSVC flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    
    # Enable SIMD optimizations
    if(ENABLE_SIMD)
        include(CheckCXXCompilerFlag)
        check_cxx_compiler_flag("/arch:AVX" AVX_SUPPORTED)
        check_cxx_compiler_flag("/arch:AVX2" AVX2_SUPPORTED)
        
        if(AVX2_SUPPORTED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX2")
        elseif(AVX_SUPPORTED)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX")
        endif()
    endif()
endif()

# Options
option(ENABLE_SIMD "Enable SIMD optimizations" ON)
option(ENABLE_OPENMP "Enable OpenMP for parallel processing" ON)
option(BUILD_TESTS "Build unit tests" ON)
option(USE_JSON "Use JSON library for logging" OFF)

# Find dependencies
find_package(Threads REQUIRED)
if(ENABLE_OPENMP)
    find_package(OpenMP)
endif()

# Optional JSON library
if(USE_JSON)
    find_package(nlohmann_json CONFIG)
    if(nlohmann_json_FOUND)
        add_definitions(-DCACA_USE_JSON)
    else()
        message(WARNING "nlohmann_json not found, falling back to simple logging")
    endif()
endif()

# Find argparse library or use FetchContent
# First try to find it as a package
find_package(argparse CONFIG QUIET)

if(NOT argparse_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        argparse
        GIT_REPOSITORY https://github.com/p-ranav/argparse.git
        GIT_TAG v2.9
    )
    FetchContent_MakeAvailable(argparse)
endif()

# Define include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Rest of the CMakeLists.txt remains the same

# Define source files
set(SOURCES
    src/main.cpp
    src/simd/cpu_features.cpp
    src/simd/dispatcher.cpp
    src/ca/cellular_automata.cpp
    src/tests/nist_tests.cpp
    src/monitoring/progress_monitor.cpp
    src/core/error_handling.cpp  # Added this file
)

# Define header files
set(HEADERS
    include/caca/core/bitsequence.hpp
    include/caca/simd/cpu_features.hpp
    include/caca/simd/dispatcher.hpp
    include/caca/ca/cellular_automata.hpp
    include/caca/tests/statistical_tests.hpp
    include/caca/tests/nist_tests.hpp
    include/caca/monitoring/progress_monitor.hpp
    include/caca/core/error_handling.hpp  # Added this header
)

# Rest of the CMakeLists.txt remains the same

# Add executable
add_executable(caca_app ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(caca_app PRIVATE
    Threads::Threads
    argparse
)

if(OpenMP_CXX_FOUND)
    target_link_libraries(caca_app PRIVATE OpenMP::OpenMP_CXX)
endif()

if(nlohmann_json_FOUND)
    target_link_libraries(caca_app PRIVATE nlohmann_json::nlohmann_json)
endif()

# Installation
install(TARGETS caca_app
    RUNTIME DESTINATION bin
)

# Testing
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentation (optional)
find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(DOXYGEN_INPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
    set(DOXYGEN_OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/docs)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)
    set(DOXYGEN_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in 
                   ${DOXYGEN_CONFIG_FILE} @ONLY)
    
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif()

# Summary
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "ENABLE_SIMD: ${ENABLE_SIMD}")
message(STATUS "ENABLE_OPENMP: ${ENABLE_OPENMP}")
message(STATUS "BUILD_TESTS: ${BUILD_TESTS}")
message(STATUS "USE_JSON: ${USE_JSON}")