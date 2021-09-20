cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    opencv
    URL               https://github.com/opencv/opencv/archive/refs/tags/4.5.3.zip
    URL_HASH          MD5=e4e25de494ba3dfb676c9f51d7c853fb
    # GIT_REPOSITORY git@github.com:ocornut/imgui.git
    # GIT_TAG        e3e1fbcf025cf83413815751f7c33500e1314d57 # v1.84.2
)

set(BUILD_TESTS OFF)
set(BUILD_PERF_TESTS OFF)
set(BUILD_EXAMPLES OFF)

FetchContent_MakeAvailable(opencv)

