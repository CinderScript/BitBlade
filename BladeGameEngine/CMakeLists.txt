# BladeGameEngine/CMakeLists.txt


set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BitBladeGame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Component.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/GameObject.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageSource.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Sprite.cpp
)

add_library(BladeGameEngine STATIC ${SOURCES})

target_link_libraries(BladeGameEngine 
    PUBLIC 
        BladeCommon
)

target_include_directories(BladeGameEngine 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

