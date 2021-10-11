cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    eigen
    URL               https://gitlab.com/libeigen/eigen/-/archive/3.4.0/eigen-3.4.0.zip
    URL_HASH          MD5=994092410ba29875184f7725e0371596
)

FetchContent_MakeAvailable(eigen)
