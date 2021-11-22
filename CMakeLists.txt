cmake_minimum_required(VERSION 3.20)
project(Simple3DReconstruction)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(3rdparty/glad)
add_subdirectory(3rdparty/glfw)
add_subdirectory(3rdparty/imgui)

find_package(gflags REQUIRED)
find_package(glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(OpenMVG REQUIRED)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(assimp REQUIRED)

add_dependencies(imgui glfw)

aux_source_directory(src MAIN_SRC_DIR)
aux_source_directory(src/app APP_SRC_DIR)
aux_source_directory(src/ui UI_SRC_DIR)
aux_source_directory(src/util UTIL_SRC_DIR)
aux_source_directory(src/render RENDER_SRC_DIR)
aux_source_directory(src/element ELEMENT_SRC_DIR)
aux_source_directory(src/process PROCESS_SRC_DIR)
aux_source_directory(src/component COMPONENT_SRC_DIR)
aux_source_directory(src/view VIEW_SRC_DIR)
aux_source_directory(src/file FILE_SRC_DIR)

set(SOURCE_LIST
    ${MAIN_SRC_DIR}
    ${APP_SRC_DIR}
    ${UI_SRC_DIR}
    ${UTIL_SRC_DIR}
    ${RENDER_SRC_DIR}
    ${ELEMENT_SRC_DIR}
    ${PROCESS_SRC_DIR}
    ${COMPONENT_SRC_DIR}
    ${VIEW_SRC_DIR}
    ${FILE_SRC_DIR}
)

if(APPLE)
    # Get a list of all of the files in the data-folder.
    file(GLOB_RECURSE RES_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/resource/*"
    )
    add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${SOURCE_LIST}
        ${RES_SOURCES}
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/MacOSXBundleInfo.plist.in
    )

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/Resources/log)

    # Individually set the file's path properties.
    foreach (FILE ${RES_SOURCES})
        # Get the relative path from the data-folder to the particular file.
        file(RELATIVE_PATH NEW_FILE "${CMAKE_CURRENT_SOURCE_DIR}/resource" ${FILE})

        # Get the relative path to the file.
        get_filename_component(NEW_FILE_PATH ${NEW_FILE} DIRECTORY)

        # Set it's location inside the app package (under Resources).
        set_property(SOURCE ${FILE} PROPERTY MACOSX_PACKAGE_LOCATION "Resources/${NEW_FILE_PATH}")

        # Optional: Add the file to the 'Resources' folder group in Xcode.
        #           This also preserves folder structure.
        source_group("Resources/${NEW_FILE_PATH}" FILES "${FILE}")
    endforeach ()

else()
    add_executable(${PROJECT_NAME}
        ${SOURCE_LIST}
    )
endif()


add_dependencies(${PROJECT_NAME} glad glfw imgui)


find_path(libuvc_INCLUDE_DIR libuvc/libuvc.h)
find_library(libuvc_LIBRARY NAMES uvc)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libuvc  DEFAULT_MSG
                                  libuvc_LIBRARY libuvc_INCLUDE_DIR)

mark_as_advanced(libuvc_INCLUDE_DIR libuvc_LIBRARY)

set(libuvc_LIBRARIES ${libuvc_LIBRARY} )
set(libuvc_INCLUDE_DIRS ${libuvc_INCLUDE_DIR})


set(3RD_PARTY_DIR build/_deps)

target_include_directories(${PROJECT_NAME}
    PUBLIC 3rdparty/glad/include
    PUBLIC 3rdparty/stb
    PUBLIC 3rdparty/imgui/include
    PUBLIC 3rdparty/imgui-filebrowser
    # PUBLIC 3rdparty/ImGuiFileDialog
    PUBLIC 3rdparty/glm
    PUBLIC ${3RD_PARTY_DIR}/glfw-src/include
    # PUBLIC ${3RD_PARTY_DIR}/glog-build
    # PUBLIC ${3RD_PARTY_DIR}/glog-src/src
    # PUBLIC ${3RD_PARTY_DIR}/gflags-build/include
    PUBLIC include
    PUBLIC ${gflags_INCLUDE_DIR}
    # PUBLIC ${glog_INCLUDE_DIR}
    PUBLIC ${OpenCV_INCLUDE_DIRS}
    PUBLIC ${OPENMVG_INCLUDE_DIRS}
    PUBLIC ${libuvc_INCLUDE_DIRS}
    PUBLIC ${Boost_INCLUDE_DIRS}
    PUBLIC ${ASSIMP_INCLUDE_DIRS}
)


target_link_directories(${PROJECT_NAME}
    PUBLIC build/3rdparty/glad
    PUBLIC build/3rdparty/imgui
    # PUBLIC build/3rdparty/ImGuiFileDialog
    PUBLIC ${3RD_PARTY_DIR}/glfw-build/src
    # PUBLIC ${3RD_PARTY_DIR}/glog-build
    # PUBLIC ${3RD_PARTY_DIR}/gflags-build
    # PUBLIC ${libuvc_LIBRARIES}
    PUBLIC ${ASSIMP_LIBRARY_DIRS}
)

# target_link_libraries(${PROJECT_NAME}
#     ${gflags_LIBS}
# )

# get_target_property(gflags_OUTPUT_NAME gflags OUTPUT_NAME)
# if (OS_WINDOWS)
#     set (gflags_OUTPUT_NAME ${gflags_OUTPUT_NAME}_static)
# endif ()

target_link_libraries(${PROJECT_NAME}
    glad
    glfw3
    imgui
    # gflags
    glog
    # ImGuiFileDialog
    # glogd
    ${gflags_LIBRARIES}
    # ${glog_LIBRARIES}
    ${OpenCV_LIBS}
    ${OPENMVG_LIBRARIES}
    # ${gflags_OUTPUT_NAME}_debug
    ${libuvc_LIBRARIES}
    ${Boost_LIBRARIES}
    ${ASSIMP_LIBRARIES}
)

if (APPLE)
  target_link_libraries(${PROJECT_NAME} "-framework Cocoa -framework IOKit")
endif()

# SET(GLFW_BUILD_DOCS OFF CACHE INTERNAL "HIDE GLFW_BUILD_DOCS" FORCE)



if(APPLE)

else()
    file(COPY ./assets/ DESTINATION ${CMAKE_BINARY_DIR}/bin/assets)
endif()