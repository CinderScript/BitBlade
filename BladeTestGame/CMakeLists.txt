set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/TestGame.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

set(COMPONENT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/components/src/Hero.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/components/src/ObjectUpdatePrinter.cpp
)

add_executable(TestGame ${SOURCES} ${COMPONENT_SOURCES})

# Link against the BitBlade libraries
target_link_libraries(TestGame 
    PRIVATE 
        BladeCommon
        BladeGameEngine
        BladeConsole
)

# Include directories for TestGame
target_include_directories(TestGame 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/components/include
)
