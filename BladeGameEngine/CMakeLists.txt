# BladeGameEngine/CMakeLists.txt

project(BladeGameEngine)

# Add the source files for the console
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BitBladeGame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sprite.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/BladeConsole/src/BladeConsole.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BladeConsole/src/windows/ConsoleLink.cpp
)

add_library(BladeGameEngine STATIC ${SOURCES})

target_link_libraries(BladeGameEngine 
    PUBLIC 
        Common
)

target_include_directories(BladeGameEngine 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/BladeConsole/include
        ${CMAKE_CURRENT_SOURCE_DIR}/BladeConsole/include/windows
)

