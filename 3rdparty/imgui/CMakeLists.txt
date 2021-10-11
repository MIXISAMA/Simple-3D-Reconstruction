cmake_minimum_required(VERSION 3.20)
project(imgui)

include(FetchContent)
FetchContent_GetProperties(glfw)

add_library(imgui STATIC
  src/imgui.cpp 
  src/imgui_draw.cpp 
  src/imgui_tables.cpp 
  src/imgui_widgets.cpp 
  src/imgui_impl_glfw.cpp
  src/imgui_impl_opengl3.cpp
)

target_include_directories(imgui
    PUBLIC ./include
    PUBLIC ${glfw_SOURCE_DIR}/include
)

target_link_directories(imgui PUBLIC ${glfw_BINARY_DIR}/src)
target_link_libraries(imgui glfw3)

if (APPLE)
    target_link_libraries(imgui "-framework Cocoa -framework IOKit")
endif()

