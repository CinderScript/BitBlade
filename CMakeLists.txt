# Top-Level CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project ("BitBlade")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(BitBladeConsole)

add_executable(BitBlade "BitBlade.cpp" "BitBlade.h")

# Include directories for all headers in BitBladeConsole
target_include_directories(BitBlade PRIVATE BitBladeConsole)

target_link_libraries(BitBlade PRIVATE BitBladeConsole)
target_link_libraries(BitBlade PRIVATE user32 gdi32)