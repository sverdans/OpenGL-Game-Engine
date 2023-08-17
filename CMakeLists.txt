cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_NAME OpenGL-Game-Engine)
set(BUILD_SHARED_LIBS OFF)

project(${PROJECT_NAME} LANGUAGES CXX)
option(ASAN "" ON)

if(WIN32)
    set(BUILD_PLATFORM PLATFORM_WIN32)
elseif(APPLE)
    set(BUILD_PLATFORM PLATFORM_MACOS)
elseif(UNIX AND NOT APPLE)
    set(BUILD_PLATFORM PLATFORM_LINUX)
endif()

message(STATUS "Building on platform : ${BUILD_PLATFORM}")

function(fetch_dependencies)
    include(FetchContent)
    set(LIBS_DIR ${CMAKE_BINARY_DIR}/external)
    message(STATUS "Fetching project dependency - start")

    FetchContent_Declare(spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        SOURCE_DIR ${LIBS_DIR}/spdlog
        GIT_TAG  v1.11.0)
    FetchContent_MakeAvailable(spdlog)
    
    set(STB_DIR ${LIBS_DIR}/stb)
    FetchContent_Declare(stb
        GIT_REPOSITORY https://github.com/nothings/stb.git
        SOURCE_DIR ${STB_DIR})
    FetchContent_MakeAvailable(stb)
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

    set(JSON_BuildTests OFF CACHE INTERNAL "")
    FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        SOURCE_DIR ${LIBS_DIR}/nlohmann
        GIT_TAG v3.11.2)
    FetchContent_MakeAvailable(json)

    set(IMGUI_DIR ${LIBS_DIR}/imgui)
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

    target_include_directories(imgui PUBLIC ${IMGUI_DIR})
    target_link_libraries(imgui PRIVATE glad glfw)

    FetchContent_Declare(nfd
        GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
        SOURCE_DIR ${LIBS_DIR}/nfd)
    FetchContent_MakeAvailable(nfd)

    message(STATUS "Fetching project dependency - done")
endfunction()

fetch_dependencies()

file(GLOB_RECURSE SOURCE "source/*.cpp")
file(GLOB_RECURSE INCLUDE "source/*.h")

add_executable(${PROJECT_NAME} ${SOURCE})

set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "engine.exe")

target_include_directories(${PROJECT_NAME} PUBLIC
    ${LIBS_DIR}/imgui
    ${LIBS_DIR}/spdlog/include
    ${LIBS_DIR}/assimp/include
    ${LIBS_DIR}/nfd/src/include
    ${LIBS_DIR}/nlohmann/include
    ${CMAKE_SOURCE_DIR}/source)

target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    spdlog
    assimp
    imgui
    glfw
    glad
    glm
    nfd)

if(${ASAN})
    message(STATUS "Building with Adress Sanitizer")
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

file(GLOB INCLUDES_COMPONENT "source/Game/Components/*.h")

target_precompile_headers(${PROJECT_NAME} PUBLIC ${INCLUDES_COMPONENT})
