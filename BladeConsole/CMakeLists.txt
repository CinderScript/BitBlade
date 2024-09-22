# BladeConsole/CMakeLists.txt

# Add the source files for the console
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BladeConsole.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/ConsoleLink.cpp
)

add_library(BladeConsole STATIC ${SOURCES})

target_link_libraries(BladeConsole 
    PRIVATE 
        BladeCommon
)

target_include_directories(BladeConsole 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/windows
)