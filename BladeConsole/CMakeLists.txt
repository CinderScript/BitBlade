# BitBladeConsole/CMakeLists.txt

project(BitBladeConsole)

# Add the source files for the console
set(SOURCES
    "main.cpp" 
    "src/BladeConsole.cpp"
    "src/GameEngine.cpp"
    "../Common/src/windows/BladeLink.cpp"
)

add_executable(BitBladeConsole ${SOURCES})

target_include_directories(BitBladeConsole PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)