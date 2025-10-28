cmake_minimum_required(VERSION 3.10)
project(caca)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cpp
    src/ca_analyzer.cpp
)

set(CORE_SOURCES
    src/bitsequence.cpp
    src/common.cpp
    src/test_suite.cpp
    src/nist_sts/generator_factory.cpp
    src/nist_sts.cpp
)

# Add test implementations explicitly
set(TEST_SOURCES
    src/nist_sts/tests/frequency_test.cpp
    src/nist_sts/tests/block_frequency_test.cpp 
    src/nist_sts/tests/runs_test.cpp
    src/nist_sts/tests/longest_run_test.cpp
    src/nist_sts/tests/rank_test.cpp
    src/nist_sts/tests/dft_test.cpp
    src/nist_sts/tests/non_overlapping_template_test.cpp
    src/nist_sts/tests/overlapping_template_test.cpp
    src/nist_sts/tests/universal_test.cpp
    src/nist_sts/tests/linear_complexity_test.cpp
    src/nist_sts/tests/serial_test.cpp
    src/nist_sts/tests/approximate_entropy_test.cpp
    src/nist_sts/tests/cumulative_sums_test.cpp
    src/nist_sts/tests/random_excursions_test.cpp
    src/nist_sts/tests/random_excursions_variant_test.cpp
)

# Collect generator sources
file(GLOB GENERATOR_SOURCES 
    "src/generators/*.cpp"
)

# Remove specific problematic files if they exist in multiple places
list(REMOVE_ITEM GENERATOR_SOURCES 
    "${CMAKE_SOURCE_DIR}/src/generators/lcg.cpp"
)

# Combine all sources
set(ALL_SOURCES 
    ${CORE_SOURCES} 
    ${TEST_SOURCES} 
    ${GENERATOR_SOURCES}
    src/ca_analyzer.cpp
)

# Create library
add_library(nist_sts STATIC ${ALL_SOURCES})

# Set include directories for the library
target_include_directories(nist_sts PUBLIC 
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/tests
    ${CMAKE_SOURCE_DIR}/src/generators
)

set(CMAKE_BUILD_PARALLEL_LEVEL 6)

# Create executable
add_executable(nist_sts_app src/main.cpp)
target_link_libraries(nist_sts_app nist_sts)

# Optional: Suppress specific warnings
if(MSVC)
    target_compile_options(nist_sts_app PRIVATE 
        /wd4267 /wd4244 /wd4805 /wd4101
    )
endif()