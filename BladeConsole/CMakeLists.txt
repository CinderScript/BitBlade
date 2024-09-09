# BitBladeConsole/CMakeLists.txt

project(BitBladeConsole)

# Add the source files for the console
set(SOURCES
    "main.cpp" 
    "src/BladeConsole.cpp"
    "src/windows/ConsoleLink.cpp"

    "BladeGameEngine/src/BitBladeGame.cpp"
    "BladeGameEngine/src/Prefab.cpp"
    "BladeGameEngine/src/GameObject.cpp"


    "BladeTestGame/src/TestGame.cpp"
)

add_executable(BitBladeConsole ${SOURCES})

target_include_directories(BitBladeConsole PRIVATE
    "include"
    "include/windows"
    "BladeGameEngine/include"

    "BladeTestGame/include"
)