cmake_minimum_required(VERSION 3.20)
project(imgui)

# link_directories(/Users/zhangjunbo/cppsource/glfw-3.3.4/clang/src)
# link_libraries(libglfw3.a)
# include_directories(. /Users/zhangjunbo/cppsource/glfw-3.3.4/include)
# list(APPEND SOURCE_LIST backends/imgui_impl_glfw.cpp backends/imgui_impl_opengl3.cpp)

# aux_source_directory(. SOURCE_LIST)

set(SOURCE_LIST
    imgui.cpp 
    imgui_draw.cpp 
    imgui_tables.cpp 
    imgui_widgets.cpp 
)

add_library(imgui SHARED ${SOURCE_LIST})

if (APPLE)
  target_link_libraries(imgui "-framework Cocoa -framework IOKit")
endif()