# Top-Level CMakeLists.txt

cmake_minimum_required(VERSION 3.8)
project ("BitBlade")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the Common directory
include_directories(
	"Common/include" 
)

add_subdirectory(BladeConsole)
add_subdirectory(BladeGraphics)
