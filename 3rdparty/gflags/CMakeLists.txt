cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    gflags
    URL               https://github.com/gflags/gflags/archive/refs/tags/v2.2.2.zip
    URL_HASH          MD5=ff856ff64757f1381f7da260f79ba79b
)

FetchContent_MakeAvailable(gflags)
