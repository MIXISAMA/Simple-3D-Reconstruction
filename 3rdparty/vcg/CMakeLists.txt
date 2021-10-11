cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    vcg
    URL               https://github.com/cnr-isti-vclab/vcglib/archive/refs/tags/2021.07.zip
    URL_HASH          MD5=d41d8cd98f00b204e9800998ecf8427e
)

FetchContent_MakeAvailable(vcg)
