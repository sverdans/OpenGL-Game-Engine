cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

set(project_name OpenGL-Courswork)

project(${project_name})

add_executable(
	${project_name}
	
	source/Editor/ImGuiFileDialog/ImGuiFileDialog.cpp

	source/main.cpp

	source/System/Parser.h
	source/System/Parser.cpp

	source/System/Time.h
	source/System/Time.cpp

	source/System/Constants.h

	source/System/Window.h
	source/System/Window.cpp

	source/System/InputHandler.h
	source/System/InputHandler.cpp

	source/Editor/UiHandler.h
	source/Editor/UiHandler.cpp
	source/Editor/Editor.h
	source/Editor/Editor.cpp

	external/stb_image.h

	source/Renderer/ShaderProgram.h
	source/Renderer/ShaderProgram.cpp

	source/Renderer/IndexBuffer.h
	source/Renderer/VertexBuffer.h
	source/Renderer/VertexArray.h
	source/Renderer/VertexBufferLayout.h

	source/Renderer/Texture.h
	source/Renderer/Texture.cpp
	source/Renderer/Material.h
	source/Renderer/Material.cpp
	
	source/Renderer/Mesh.h
	source/Renderer/Mesh.cpp
	source/Renderer/Model.h
	source/Renderer/Model.cpp

	source/Renderer/Renderer.h
	source/Renderer/Renderer.cpp

	source/Resources/ResourceManager.h
	source/Resources/ResourceManager.cpp

	source/Game/ObjectsManager.h
	source/Game/ObjectsManager.cpp
	source/Game/GameObject.h
	source/Game/GameObject.cpp

	source/Game/Systems/RendererSystem.h
	source/Game/Systems/RendererSystem.cpp
	source/Game/Systems/LightingSystem.h
	source/Game/Systems/LightingSystem.cpp
	source/Game/Systems/BehaviourSystem.h
	source/Game/Systems/BehaviourSystem.cpp

	source/Game/Components/Component.h
	source/Game/Components/TransformComponent.h
	source/Game/Components/TransformComponent.cpp

	source/Game/Components/RendererComponent.h
	source/Game/Components/RendererComponent.cpp
	source/Game/Components/ModelRendererComponent.h
	source/Game/Components/ModelRendererComponent.cpp
	source/Game/Components/LightingComponent.h
	source/Game/Components/LightingComponent.cpp

	source/Game/Components/BehaviourComponent.h
	source/Game/Components/BehaviourComponent.cpp
	source/Game/Components/CameraComponent.h
	source/Game/Components/CameraComponent.cpp

	source/Game/Components/L4T1_Component.h
	source/Game/Components/L4T1_Component.cpp

	source/Game/Components/L4T3_Component.h
	source/Game/Components/L4T3_Component.cpp

	source/Game/Components/L4T4_Component.h
	source/Game/Components/L4T4_Component.cpp

	source/Game/Components/ClockComponent.h
	source/Game/Components/ClockComponent.cpp
)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(external/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

add_subdirectory(external/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp)

include_directories(external/json/include)

set(IMGUI_INCLUDES
	external/imgui/imgui.h
	external/imgui/backends/imgui_impl_glfw.h
	external/imgui/backends/imgui_impl_opengl3.h
)

set(IMGUI_SOURCES
	external/imgui/imgui.cpp
	external/imgui/backends/imgui_impl_glfw.cpp
	external/imgui/backends/imgui_impl_opengl3.cpp
	external/imgui/imgui_demo.cpp
	external/imgui/imgui_tables.cpp
	external/imgui/imgui_widgets.cpp
	external/imgui/imgui_draw.cpp
)

add_library(ImGui STATIC
	${IMGUI_INCLUDES}
	${IMGUI_SOURCES}
)

target_include_directories(ImGui PUBLIC external)
target_include_directories(ImGui PUBLIC external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGui)
