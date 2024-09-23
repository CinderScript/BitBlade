# tests/CMakeLists.txt

# Enable CMake's FetchContent module
include(FetchContent)

# Fetch Google Test
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Fetch and make available
FetchContent_MakeAvailable(googletest)

# Add test to CTest
include(GoogleTest)


add_subdirectory(BladeGameEngine)

