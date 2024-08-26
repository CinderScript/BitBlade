# BitBladeConsole/CMakeLists.txt

project(BitBladeConsole)

# Add the source files for the console
set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BladeConsole.cpp"
    "main.cpp")

add_executable(BitBladeConsole ${SOURCES})

target_include_directories(BitBladeConsole PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)