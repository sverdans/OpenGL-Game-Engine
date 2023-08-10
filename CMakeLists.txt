cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_NAME OpenGL-Game-Engine)
set(BUILD_SHARED_LIBS OFF)

project(${PROJECT_NAME} LANGUAGES CXX)
option(ASAN "" ON)

if(${ASAN})
    message(STATUS "Building with Adress Sanitizer")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fno-omit-frame-pointer -fsanitize=address")
endif()

function(fetch_dependencies)
    message(STATUS "Fetching project dependency - start")

    include(FetchContent)
    set(LIBS_DIR ${CMAKE_BINARY_DIR}/external)

    set(STB_DIR ${LIBS_DIR}/stb)
    FetchContent_Declare(stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        SOURCE_DIR ${STB_DIR})
    FetchContent_MakeAvailable(stb)

    file(GLOB_RECURSE STB_FILES "${STB_DIR}/*")
    foreach(STB_FILE_PATH ${STB_FILES})
        string(FIND ${STB_FILE_PATH} "stb_image.h" EXCLUDE_FILE_INDEX)
        if(NOT ${EXCLUDE_FILE_INDEX} EQUAL -1)
            message(STATUS "NOT REMOVABLE FILE: ${STB_FILE_PATH}")
        elseif()
            file(REMOVE_RECURSE ${STB_FILE_PATH})
        endif()
    endforeach()
    include_directories(${LIBS_DIR}/stb)

    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        SOURCE_DIR ${LIBS_DIR}/glfw)
    FetchContent_MakeAvailable(glfw)
    include_directories(${LIBS_DIR}/glfw/include)

    FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        SOURCE_DIR ${LIBS_DIR}/glad)
    FetchContent_MakeAvailable(glad)
    include_directories(${GLAD_BINARY_DIR}/include)

    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        SOURCE_DIR ${LIBS_DIR}/glm)
    FetchContent_MakeAvailable(glm)
    include_directories(${LIBS_DIR}/glm)

    FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        SOURCE_DIR ${LIBS_DIR}/assimp)
    FetchContent_MakeAvailable(assimp)
    include_directories(${LIBS_DIR}/assimp/include)

    set(JSON_BuildTests OFF CACHE INTERNAL "")
    FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        SOURCE_DIR ${LIBS_DIR}/nlohmann
        GIT_TAG v3.11.2)
    FetchContent_MakeAvailable(json)
    include_directories(${LIBS_DIR}/nlohmann/single_include)

    set(IMGUI_DIR ${LIBS_DIR}/imgui/imgui)
    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        SOURCE_DIR ${IMGUI_DIR})
    FetchContent_MakeAvailable(imgui)

    set(IMGUI_INCLUDES
        ${IMGUI_DIR}/imgui.h
        ${IMGUI_DIR}/backends/imgui_impl_glfw.h
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.h)

    set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_draw.cpp)

    add_library(imgui STATIC ${IMGUI_INCLUDES} ${IMGUI_SOURCES})

    set_target_properties(imgui PROPERTIES 
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-build
        ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/_deps/imgui-build)

    include_directories(${IMGUI_DIR})
    include_directories(${LIBS_DIR}/imgui)
    target_link_libraries(imgui PRIVATE glad glfw)

    message(STATUS "Fetching project dependency - done")
endfunction()

fetch_dependencies()

file(GLOB_RECURSE SOURCE "source/*.cpp")
file(GLOB_RECURSE INCLUDE "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "engine.exe")

include_directories(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/include
                                    ${CMAKE_SOURCE_DIR}/source)
    
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm
    assimp
    imgui)
