# TestGame/CMakeLists.txt

project(TestGame)

# Add the source files for the console
set(SOURCES
    "src/TestGame.cpp"
	"main.cpp"
)

add_executable(TestGame ${SOURCES})

target_include_directories(TestGame PRIVATE
    "include"
)

# Link against the BladeGameEngine library
target_link_libraries(TestGame PRIVATE BitBladeGameEngine)