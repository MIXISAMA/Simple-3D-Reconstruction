cmake_minimum_required(VERSION 3.20)
project(Simple3DReconstruction)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/imgui)
# add_subdirectory(3rdparty/opencv)
add_subdirectory(3rdparty/gflags)
add_subdirectory(3rdparty/glog)
add_subdirectory(3rdparty/glad)

add_dependencies(imgui glfw)
add_dependencies(glog gflags)

set(SOURCE_LIST
    main.cpp 
)

add_executable(${PROJECT_NAME}
    ${SOURCE_LIST}
)

set(3RD_PARTY_DIR build/_deps)

target_include_directories(${PROJECT_NAME}
    PUBLIC 3rdparty/glad/include
    PUBLIC ${3RD_PARTY_DIR}/glfw-src/include
    PUBLIC ${3RD_PARTY_DIR}/imgui-src
    PUBLIC ${3RD_PARTY_DIR}/imgui-src/backends
    PUBLIC ${3RD_PARTY_DIR}/glog-build
    PUBLIC ${3RD_PARTY_DIR}/glog-src/src
    PUBLIC ${3RD_PARTY_DIR}/gflags-build/include
)

target_link_directories(${PROJECT_NAME}
    PUBLIC build/3rdparty/glad
    PUBLIC ${3RD_PARTY_DIR}/glfw-build/src
    PUBLIC ${3RD_PARTY_DIR}/imgui-build
    PUBLIC ${3RD_PARTY_DIR}/glog-build
    PUBLIC ${3RD_PARTY_DIR}/gflags-build
)

# target_link_libraries(${PROJECT_NAME}
#     ${gflags_LIBS}
# )


target_link_libraries(${PROJECT_NAME}
    glad
    glfw3
    imgui
    glogd
    gflags_nothreads_debug
)

if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa -framework IOKit")
endif()