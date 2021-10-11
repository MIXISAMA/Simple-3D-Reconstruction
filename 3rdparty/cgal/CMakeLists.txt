cmake_minimum_required(VERSION 3.20)

include(FetchContent)

FetchContent_Declare(
    CGAL
    URL               https://github.com/CGAL/cgal/releases/download/v5.3/CGAL-5.3-library.zip
    URL_HASH          MD5=d0f6d2183542e2a809ba961b90a86dcc
)

FetchContent_GetProperties(CGAL)
if(NOT CGAL_POPULATED)
  FetchContent_Populate(CGAL)
endif()

# message("wtf")
# message(${CGAL_SOURCE_DIR})

# file(COPY ./patch/CMakeLists.txt DESTINATION ${CGAL_SOURCE_DIR})
# add_subdirectory(${CGAL_SOURCE_DIR} ${CGAL_BINARY_DIR})
