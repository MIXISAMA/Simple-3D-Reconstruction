cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    glog
    URL               https://github.com/google/glog/archive/refs/tags/v0.5.0.zip
    URL_HASH          MD5=2286d65cc3714b26e085a3bc62ce38a5
)
FetchContent_GetProperties(gflags)
set(gflags_DIR ${gflags_BINARY_DIR})
set(BUILD_TESTING OFF)
set(WITH_GTEST OFF)
set(HAVE_SNPRINTF TRUE)

FetchContent_GetProperties(glog)
if(NOT glog_POPULATED)
  FetchContent_Populate(glog)
endif()

file(COPY ./patch/CMakeLists.txt DESTINATION ${glog_SOURCE_DIR})
add_subdirectory(${glog_SOURCE_DIR} ${glog_BINARY_DIR})

