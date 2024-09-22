# Top-Level CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project ("BitBlade")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# Enable testing
enable_testing()

# Option to build tests
option(BUILD_TESTS "Build tests on Debug" ON)

add_subdirectory(BladeCommon)
add_subdirectory(BladeConsole)
add_subdirectory(BladeGameEngine)
add_subdirectory(BladeGraphics)

add_subdirectory(BladeTestGame)

# Conditionally add tests
if(BUILD_TESTS AND (CMAKE_BUILD_TYPE STREQUAL "Debug"))
    enable_testing()
    add_subdirectory(tests)
endif()