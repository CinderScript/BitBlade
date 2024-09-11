# BladeGameEngine/CMakeLists.txt

project(BitBladeGameEngine)

# Add the source files for the console
set(SOURCES
    "src/BitBladeGame.cpp"
    "src/Prefab.cpp"
    "src/GameObject.cpp"

    "BladeConsole/src/BladeConsole.cpp"
    "BladeConsole/src/windows/ConsoleLink.cpp"
)

add_library(BitBladeGameEngine ${SOURCES})

target_include_directories(BitBladeGameEngine PUBLIC
    "include"
    "BladeConsole/include"
    "BladeConsole/include/windows"
)