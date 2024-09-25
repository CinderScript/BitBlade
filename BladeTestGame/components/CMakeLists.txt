# TestGame/components/CMakeLists.txt

# Add the source files for the console
set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Hero.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ObjectUpdatePrinter.cpp
)  


target_include_directories(TestGame 
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
