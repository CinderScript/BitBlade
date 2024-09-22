# BladeCommon/CMakeLists.txt

add_library(BladeCommon INTERFACE)

target_include_directories(BladeCommon 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
