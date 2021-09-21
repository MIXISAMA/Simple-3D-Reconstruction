cmake_minimum_required(VERSION 3.20)
project(imgui)

set(SOURCE_LIST
    imgui.cpp 
    imgui_draw.cpp 
    imgui_tables.cpp 
    imgui_widgets.cpp 
)

list(APPEND SOURCE_LIST
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

include(FetchContent)
FetchContent_GetProperties(glfw)

add_library(imgui STATIC ${SOURCE_LIST})
target_link_directories(imgui PUBLIC ${glfw_BINARY_DIR}/src)
target_link_libraries(imgui glfw3)
target_include_directories(imgui
    PUBLIC .
    PUBLIC ./backends
    PUBLIC ${glfw_SOURCE_DIR}/include
)



if (APPLE)
    target_link_libraries(imgui "-framework Cocoa -framework IOKit")
endif()

