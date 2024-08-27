# BladeGraphics/CMakeLists.txt

project(BitBladeGraphics)

# Add the source files for the console
set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/BladeGraphics.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/ImageData.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/DisplayDriver.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/PixelOperations.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/windows/ImageLoader.cpp" 
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" 
    "src/MasterSpriteData.cpp"
    "src/SpriteInstance.cpp")

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