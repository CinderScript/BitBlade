# BladeGraphics/CMakeLists.txt

project(BitBladeGraphics)

# Add the source files for the console
set(SOURCES
    "main.cpp" 
    "src/MasterSpriteData.cpp"
    "src/SpriteInstance.cpp"
    "src/BladeGraphics.cpp" 
    "src/ImageData.cpp" 
    "src/windows/DisplayDriver.cpp"
    "src/windows/PixelOperations.cpp"
    "src/windows/ImageLoader.cpp"
    "src/windows/ConsoleConnector.cpp"
)

add_executable(BitBladeGraphics ${SOURCES})

target_link_libraries(BitBladeGraphics PRIVATE user32 gdi32)

target_include_directories(BitBladeGraphics PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

if (WIN32)
    target_include_directories(BitBladeGraphics PRIVATE 
        "${CMAKE_CURRENT_SOURCE_DIR}/stb-master"
    )
endif()