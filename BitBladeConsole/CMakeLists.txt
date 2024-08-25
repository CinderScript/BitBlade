# BitBladeConsole/CMakeLists.txt

project(BitBladeConsole)

# Add the source files for the console
set(SOURCES
    "BladeConsole.cpp"
    "BladeGraphics/src/windows/DisplayDriver.cpp"
    "BladeGraphics/src/windows/PixelOperations.cpp"
    "BladeGraphics/src/ImageBuffer.cpp" 
    "BladeGraphics/src/BladeGraphics.cpp")

# Create the BitBladeConsole object library
add_library(BitBladeConsole OBJECT ${SOURCES})

target_include_directories(BitBladeConsole PRIVATE 
    "${CMAKE_CURRENT_SOURCE_DIR}/BladeGraphics/include"
)
