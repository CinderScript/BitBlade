# tests/GameEngine/CMakeLists.txt

# Create a list of test sources
file(GLOB GAME_ENGINE_TEST_SOURCES "src/*.cpp")

# Add an executable for tests
add_executable(GameEngine_tests ${GAME_ENGINE_TEST_SOURCES})

# Link against Google Test and your project libraries
target_link_libraries(GameEngine_tests 
    GTest::gtest 
    GTest::gtest_main 
    BladeGameEngine
)

target_include_directories(GameEngine_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


gtest_discover_tests(GameEngine_tests)