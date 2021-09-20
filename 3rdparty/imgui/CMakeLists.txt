cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    imgui
    URL               https://github.com/ocornut/imgui/archive/refs/tags/v1.84.2.zip
    URL_HASH          MD5=9ee6941a3ee7e612b4dc0bcf06d5a265
    # GIT_REPOSITORY git@github.com:ocornut/imgui.git
    # GIT_TAG        e3e1fbcf025cf83413815751f7c33500e1314d57 # v1.84.2
)

FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_Populate(imgui)
endif()

file(COPY ./patch/CMakeLists.txt DESTINATION ${imgui_SOURCE_DIR})
add_subdirectory(${imgui_SOURCE_DIR} ${imgui_BINARY_DIR})
