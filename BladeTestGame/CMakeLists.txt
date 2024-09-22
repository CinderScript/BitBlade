# TestGame/CMakeLists.txt

# Add the source files for the console
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestGame.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

add_executable(TestGame ${SOURCES})


# Link against the BladeGameEngine library
target_link_libraries(TestGame 
    PRIVATE 
        BladeCommon
        
        BladeGameEngine
        BladeConsole
)     


target_include_directories(TestGame 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
