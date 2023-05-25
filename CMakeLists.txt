cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_NAME OpenGL-Game-Engine)

option(FETCH_DEPENDENCIES "" ON)

project(${PROJECT_NAME})

if(NOT PKG_CONFIG_FOUND)
    include(CMakeFindDependencyMacro)
    find_dependency(PkgConfig)
endif()

if(${FETCH_DEPENDENCIES})

    include(FetchContent)

    message([STATUS] " DOWNLOAD_COMMAND start")

    set(LIBS_DIR ${CMAKE_BINARY_DIR}/external)
    
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        SOURCE_DIR ${LIBS_DIR}/glfw)
    FetchContent_MakeAvailable(glfw)

    FetchContent_Declare(glad
        GIT_REPOSITORY https://github.com/Dav1dde/glad.git
        SOURCE_DIR ${LIBS_DIR}/glad)
    FetchContent_MakeAvailable(glad)

    FetchContent_Declare(glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        SOURCE_DIR ${LIBS_DIR}/glm)
    FetchContent_MakeAvailable(glm)

    FetchContent_Declare(assimp
        GIT_REPOSITORY https://github.com/assimp/assimp.git
        SOURCE_DIR ${LIBS_DIR}/assimp)
    FetchContent_MakeAvailable(assimp)

    FetchContent_Declare(json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        SOURCE_DIR ${LIBS_DIR}/nlohmann
        GIT_TAG v3.7.3)
    FetchContent_MakeAvailable(json)

    FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        SOURCE_DIR ${LIBS_DIR}/imgui)
    FetchContent_MakeAvailable(imgui)
    
    set(IMGUI_INCLUDES
        ${LIBS_DIR}/imgui/imgui.h
        ${LIBS_DIR}/imgui/backends/imgui_impl_glfw.h
        ${LIBS_DIR}/imgui/backends/imgui_impl_opengl3.h)

    set(IMGUI_SOURCES
        ${LIBS_DIR}/imgui/imgui.cpp
        ${LIBS_DIR}/imgui/backends/imgui_impl_glfw.cpp
        ${LIBS_DIR}/imgui/backends/imgui_impl_opengl3.cpp
        ${LIBS_DIR}/imgui/imgui_demo.cpp
        ${LIBS_DIR}/imgui/imgui_tables.cpp
        ${LIBS_DIR}/imgui/imgui_widgets.cpp
        ${LIBS_DIR}/imgui/imgui_draw.cpp)

    add_library(imgui STATIC ${IMGUI_INCLUDES} ${IMGUI_SOURCES})

    target_include_directories(imgui PUBLIC ${LIBS_DIR}/imgui)
    target_link_libraries(imgui PRIVATE glad glfw)

    message([STATUS] " DOWNLOAD_COMMAND done")

endif()

file(GLOB_RECURSE SOURCE "source/*.h" "source/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE})

target_include_directories(${PROJECT_NAME} PUBLIC 
    ${LIBS_DIR}/nlohmann/include
    ${LIBS_DIR}
    ${LIBS_DIR}/glm
    ${LIBS_DIR}/glfw/include
    ${LIBS_DIR}/assimp/include
    ${GLAD_BINARY_DIR}/include)
    
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glad
    glm
    assimp
    imgui)
