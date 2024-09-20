# BladeGraphics/CMakeLists.txt

project(BladeGraphics)

# Add the source files for the console
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BladeGraphics.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/BladeGraphicsProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MasterSpriteData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/SpriteInstance.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImageData.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/DisplayDriver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/PixelOperations.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/ImageLoader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/windows/GraphicsLink.cpp
)

add_executable(BladeGraphics ${SOURCES})

target_link_libraries(BladeGraphics 
    PRIVATE 
        Common

        user32 
        gdi32
)

target_include_directories(BladeGraphics 
    PRIVATE
        Common
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/windows
)

if (WIN32)
    target_include_directories(BladeGraphics PRIVATE 
        "stb-master"
    )
endif()