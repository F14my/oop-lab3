cmake_minimum_required(VERSION 3.20)
project(lab_03 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

add_library(complex_set
    src/Complex.cpp
    src/ComplexSet.cpp
)
target_include_directories(complex_set PUBLIC include)
if(MSVC)
    target_compile_options(complex_set PRIVATE /W4)
else()
    target_compile_options(complex_set PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(lab_03 src/main.cpp)
target_link_libraries(lab_03 PRIVATE complex_set)

find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
endif()

enable_testing()
add_executable(lab_03_tests tests/tests.cpp)
target_link_libraries(lab_03_tests PRIVATE complex_set GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lab_03_tests)
