cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    glfw
    URL               https://github.com/glfw/glfw/releases/download/3.3.4/glfw-3.3.4.zip
    URL_HASH          MD5=7cd3b5db36dc8c644871de51f478882f
    # GIT_REPOSITORY git@github.com:glfw/glfw.git
    # GIT_TAG        814b7929c5add4b0541ccad26fb81f28b71dc4d8 # 3.3.4
)

FetchContent_MakeAvailable(glfw)
