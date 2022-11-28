#define _USE_MATH_DEFINES

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <math.h>
#include <string>

#include "System/Window.h"
#include "System/Time.h"
#include "Editor/UiHandler.h"

#include "Game/ObjectsManager.h"
#include "Game/GameObject.h"
#include "Game/Components/TransformComponent.h"
#include "Game/Components/ShapeRendererComponent.h"
#include "Game/Components/HyperboloidRendererComponent.h"
#include "Game/Components/CameraComponent.h"

class Lab
{
private:
	static bool depthTest;
	static bool isCameraPerspective;

	static unsigned int currentLab;
	static int currentPolygonMode;

	static int pointSize;

	static const char* exercises[];
	static const char* drawModes[];
	static const char* polygonModes[];

	static int currentExercise;
	static int currentDrawMode;

	static glm::vec3 hyperboloidRotation;
	static glm::vec3 hyperboloidScale;
	static int hyperboloidLayers;
	static int hyperboloidPoints;

	static bool isFlatsNeedRender;
	static bool isTreeNeedRender;

	static Primitive* getCone(int type = 0) // type: 0 - base | 1 - crown | 2 - trunk
	{
		constexpr int anglesCount = 16;
		constexpr int vertexCount = anglesCount + 2;
		constexpr int indicesCount = anglesCount * 3 * 2;

		GLfloat vertexCoords[vertexCount * 3];
		GLuint indices[indicesCount];

		glm::vec2 center(0.f, 0.f);
		float R = 0.5f;

		vertexCoords[0] = center.x;
		vertexCoords[1] = 0.f;
		vertexCoords[2] = center.y;

		for (unsigned int i = 0; i < anglesCount; ++i)
		{
			unsigned int k = i + 1;

			vertexCoords[k * 3] = center.x + R * cos(M_PI / 2 + 2 * M_PI * i / anglesCount);
			vertexCoords[k * 3 + 1] = 0.f;
			vertexCoords[k * 3 + 2] = center.y + R * sin(M_PI / 2 + 2 * M_PI * i / anglesCount);

			indices[i * 3] = 0;
			indices[i * 3 + 1] = k;
			indices[i * 3 + 2] = (k == anglesCount ? 1 : k + 1);

			indices[(i + anglesCount) * 3] = vertexCount - 1;
			indices[(i + anglesCount) * 3 + 1] = k;
			indices[(i + anglesCount) * 3 + 2] = (k == anglesCount ? 1 : k + 1);
		}

		vertexCoords[(vertexCount - 1) * 3 ] = center.x;
		vertexCoords[(vertexCount - 1) * 3 + 1] = 1.f;
		vertexCoords[(vertexCount - 1) * 3 + 2] = center.y;

		GLfloat* colors = new GLfloat[vertexCount * 3];

		switch (type)
		{
		case 0:
			for (unsigned int i = 0; i < vertexCount; ++i)
			{
				colors[i * 3] = (30.f + rand() % 5 - 2.5f) / 255.f;
				colors[i * 3 + 1] = (180.f + rand() % 5 - 2.5f) / 255.f;
				colors[i * 3 + 2] = (30.f + rand() % 5 - 2.5f) / 255.f;
			}
			break;
		case 1:
			for (unsigned int i = 0; i < vertexCount; ++i)
			{
				colors[i * 3] = (30.f + rand() % 5 - 2.5f) / 255.f;
				colors[i * 3 + 1] = (180.f + rand() % 5 - 2.5f) / 255.f;
				colors[i * 3 + 2] = (30.f + rand() % 5 - 2.5f) / 255.f;
			}
			colors[(vertexCount - 1) * 3] = 1.f;
			colors[(vertexCount - 1) * 3 + 1] = 1.f;
			colors[(vertexCount - 1) * 3 + 2] = 1.f;
			break;

		case 2:
			for (unsigned int i = 0; i < vertexCount; ++i)
			{
				colors[i * 3] = 80.f / 255.f;
				colors[i * 3 + 1] = 30.f / 255.f;
				colors[i * 3 + 2] = 20.f / 255.f;
			}
			break;

		}

		Primitive* cone = new Primitive(indices, indicesCount, vertexCoords, vertexCount * 3, colors, GL_TRIANGLES);
		delete[] colors;
		return cone;
	}

	static Primitive* getCube()
	{
		int vertexCount = 8;
		int indicesCount = 3 * 2 * 6;

		GLfloat points[] = {
			0.f, 0.f, 0.f,
			1.f, 0.f, 0.f,
			1.f, 1.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 1.f, 1.f,
			0.f, 0.f, 1.f,
			1.f, 1.f, 1.f,
			1.f, 0.f, 1.f
		};

		GLuint cubeIndices[] = {
			0, 1, 3,
			3, 2, 1,
			1, 2, 6,
			6, 7, 1,
			7, 1, 0,
			0, 5, 7,
			0, 3, 4,
			4, 0, 5,
			5, 7, 6,
			6, 5, 4,
			4, 3, 6,
			6, 2, 3
		};

		GLfloat* colors = new GLfloat[vertexCount * 3];
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			colors[i * 3] = rand() % 255 / 255.f;
			colors[i * 3 + 1] = rand() % 255 / 255.f;
			colors[i * 3 + 2] = rand() % 255 / 255.f;
		}
		
		Primitive* cube = new Primitive(cubeIndices, indicesCount, points, vertexCount * 3, colors, GL_TRIANGLES);
		delete[] colors;
		return cube;
	}

/*--------------------LAB-2--------------------*/

	static int currentFigure;
	static int anglesCount;
	static const char* figures[];

	static void lab2_task1()
	{
		auto object = ObjectsManager::findByTag("polygon");
		auto TC = object->getComponent<TransformComponent>();
		auto PRC = object->getComponent<ShapeRendererComponent>();

		int arraySize = anglesCount * 3 + 3;
		float R = 0.5f;

		GLuint* indices = nullptr;
		GLfloat* vertexCoords = new GLfloat[arraySize];
		GLfloat* colors = new GLfloat[arraySize];

		glm::vec2 center(0.5f, 0.5f);

		vertexCoords[0] = center.x;
		vertexCoords[1] = center.y;
		vertexCoords[2] = 1.f;

		for (unsigned int i = 0; i <= anglesCount; ++i)
		{
			colors[i * 3] = rand() % 255 / 255.f;
			colors[i * 3 + 1] = rand() % 255 / 255.f;
			colors[i * 3 + 2] = rand() % 255 / 255.f;
		}

		for (unsigned int i = 0; i < anglesCount; ++i)
		{
			unsigned int k = i + 1;
			float x = center.x + R * cos(M_PI / 2 + 2 * M_PI * i / anglesCount);
			float y = center.y + R * sin(M_PI / 2 + 2 * M_PI * i / anglesCount);

			vertexCoords[k * 3] = x;
			vertexCoords[k * 3 + 1] = y;
			vertexCoords[k * 3 + 2] = 1.f;
		}

		GLenum drawMode;
		int indicesCount;

		switch (currentDrawMode)
		{
			case 0: // GL_TRIANGLES
			{
				drawMode = GL_TRIANGLES;
				indicesCount = arraySize;
				indices = new GLuint[indicesCount];
				for (unsigned int i = 0; i < anglesCount; ++i)
				{
					unsigned int k = i + 1;
					indices[i * 3] = 0;
					indices[i * 3 + 1] = k;
					indices[i * 3 + 2] = (k == anglesCount ? 1 : k + 1);
				}
				break;
			}
			case 1: // GL_TRIANGLE_FAN
			{
				drawMode = GL_TRIANGLE_FAN;
				indicesCount = anglesCount * 2;
				indices = new GLuint[indicesCount];
				indices[0] = 0;
				for (unsigned int i = 1; i < anglesCount; ++i)
				{
					indices[i * 2 - 1] = i;
					indices[i * 2] = i + 1;
				}
				indices[indicesCount - 1] = 1;
				break;
			}
			case 2: // GL_LINE_LOOP
			{
				indicesCount = anglesCount;
				indices = new GLuint[indicesCount];
				drawMode = GL_LINE_LOOP;
				for (unsigned int i = 0; i < anglesCount; ++i)
					indices[i] = i + 1;
				break;
			}
			default: return;
		}

		Primitive* polygon = new Primitive(indices, indicesCount, vertexCoords, arraySize, colors, drawMode);
		delete PRC->primitive;
		PRC->primitive = polygon;

		delete[] indices;
		delete[] colors;
		delete[] vertexCoords;
	}

	static void lab2_task2()
	{
		auto object = ObjectsManager::findByTag("polygon");
		auto TC = object->getComponent<TransformComponent>();
		auto PRC = object->getComponent<ShapeRendererComponent>();

		GLenum drawMode = GL_TRIANGLES;

		int indicesCount, vertexCount;
		GLuint* indices = nullptr;
		GLfloat* points = nullptr;
		GLfloat* colors = nullptr;

		switch (currentFigure)
		{
			case 0: // Triangle 
			{
				indicesCount = vertexCount = 3;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.0f; points[1] = 0.0f; points[2] = 1.0f;
				points[3] = 0.0f; points[4] = 1.0f; points[5] = 1.0f;
				points[6] = 1.0f; points[7] = 0.0f; points[8] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				break;
			}
			case 1: // Trapezoid
			{
				indicesCount = 6;
				vertexCount = 4;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.0f;	points[1] = 0.0f;	points[2] = 1.0f;
				points[3] = 0.2f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 0.8f;	points[7] = 1.0f;	points[8] = 1.0f;
				points[9] = 1.0f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;
				
				break;
			}
			case 2: // N-gone
			{
				indicesCount = 9;
				vertexCount = 5;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.0f;	points[1] = 0.5f;	points[2] = 1.0f;
				points[3] = 0.25f;	points[4] = 0.0f;	points[5] = 1.0f;
				points[6] = 0.5f;	points[7] = 0.5f;	points[8] = 1.0f;
				points[9] = 1.0f;	points[10] = 0.0f;	points[11] = 1.0f;
				points[12] = 1.0f;	points[13] = 1.0f;	points[14] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 1; indices[4] = 2; indices[5] = 3;
				indices[6] = 3; indices[7] = 2; indices[8] = 4;

				break;
			}
			case 3: // Parallelogram
			{
				indicesCount = 6;
				vertexCount = 4;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.0f;	points[1] = 0.0f;	points[2] = 1.0f;
				points[3] = 0.25f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 1.0f;	points[7] = 1.0f;	points[8] = 1.0f;
				points[9] = 0.75f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;
				
				break;
			}
			case 4: // Rectangle
			{
				indicesCount = 6;
				vertexCount = 4;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.0f;	points[1] = 0.25f;	points[2] = 1.0f;
				points[3] = 0.0f;	points[4] = 0.75f;	points[5] = 1.0f;
				points[6] = 1.0f;	points[7] = 0.75f;	points[8] = 1.0f;
				points[9] = 1.0f;	points[10] = 0.25f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				break;
			}
			case 5: // Deltoid
			{
				indicesCount = 6;
				vertexCount = 4;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.2f;	points[1] = 0.75f;	points[2] = 1.0f;
				points[3] = 0.5f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 0.8f;	points[7] = 0.75f;	points[8] = 1.0f;
				points[9] = 0.5f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				break;
			}
			case 6: // Rhombus
			{
				indicesCount = 6;
				vertexCount = 4;
				indices = new GLuint[indicesCount];
				points = new GLfloat[vertexCount * 3];

				points[0] = 0.25f;	points[1] = 0.5f;	points[2] = 1.0f;
				points[3] = 0.5f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 0.75f;	points[7] = 0.5f;	points[8] = 1.0f;
				points[9] = 0.5f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				break;
			}
			default: return;
		}

		colors = new GLfloat[vertexCount * 3];
		for (unsigned int i = 0; i < vertexCount; ++i)
		{
			colors[i * 3] = rand() % 255 / 255.f;
			colors[i * 3 + 1] = rand() % 255 / 255.f;
			colors[i * 3 + 2] = rand() % 255 / 255.f;
		}

		Primitive* polygon = new Primitive(indices, indicesCount, points, vertexCount * 3, colors, drawMode);
		delete PRC->primitive;
		PRC->primitive = polygon;

		delete[] indices;
		delete[] colors;
		delete[] points;
	}

	static void lab2_task4()
	{
		auto object = ObjectsManager::findByTag("polygon");
		auto TC = object->getComponent<TransformComponent>();
		auto PRC = object->getComponent<ShapeRendererComponent>();

		GLenum drawMode = GL_POINTS;

		int pointsCount = rand() % 200 + 200;
		GLuint* indices = new GLuint[pointsCount];
		GLfloat* points = new GLfloat[pointsCount * 3];
		GLfloat* colors = new GLfloat[pointsCount * 3];

		for (int i = 0; i < pointsCount; ++i)
		{
			indices[i] = i;

			colors[i * 3] = rand() % 255 / 255.f;
			colors[i * 3 + 1] = rand() % 255 / 255.f;
			colors[i * 3 + 2] = rand() % 255 / 255.f;

			points[i * 3] = rand() % 500 / 500.f;
			points[i * 3 + 1] = rand() % 500 / 500.f;
			points[i * 3 + 2] = rand() % 500 / 500.f;
		}

		Primitive* polygon = new Primitive(indices, pointsCount, points, pointsCount * 3, colors, drawMode);
		delete PRC->primitive;
		PRC->primitive = polygon;

		delete[] indices;
		delete[] colors;
		delete[] points;
	}

	static void lab2()
	{
		ImGui::NewLine();

		if (ImGui::Combo("Task", &currentExercise, exercises, 3))
		{
			switch (currentExercise)
			{
				case 0: lab2_task1(); break;
				case 1: lab2_task2(); break;
				case 2: lab2_task4(); break;
			}
		}

		switch (currentExercise)
		{
			case 0:
				if (ImGui::SliderInt("Angles", &anglesCount, 3, 12))
					lab2_task1();
				if (ImGui::Combo("Mode", &currentDrawMode, drawModes, 3))
					lab2_task1();
				break;
			case 1:
				if (ImGui::Combo("Figure", &currentFigure, figures, 7))
					lab2_task2();
				break;
			case 2:
				if (ImGui::Button("Randomize"))
					lab2_task4();
				break;
		}
	}

/*--------------------LAB-3--------------------*/
	static void lab3() 
	{

		if (ImGui::CollapsingHeader("Flats"))
		{
			if (ImGui::Checkbox("render##flats", &isFlatsNeedRender));
			{
				auto polygon1 = ObjectsManager::findByTag("polygon1");
				auto RC1 = polygon1->getComponent<ShapeRendererComponent>();
				auto polygon2 = ObjectsManager::findByTag("polygon2");
				auto RC2 = polygon2->getComponent<ShapeRendererComponent>();
				auto polygon3 = ObjectsManager::findByTag("polygon3");
				auto RC3 = polygon3->getComponent<ShapeRendererComponent>();
				
				RC1->renderEnable = RC2->renderEnable = RC3->renderEnable = isFlatsNeedRender;
			}
		}

		if (ImGui::CollapsingHeader("Cube"))
		{
			auto object = ObjectsManager::findByTag("cube");
			auto RC = object->getComponent<ShapeRendererComponent>();
			if (ImGui::Checkbox("render##cube", &RC->renderEnable));
		}

		if (ImGui::CollapsingHeader("Tree"))
		{
			if (ImGui::Checkbox("render##tree", &isTreeNeedRender))
			{
				auto object1 = ObjectsManager::findByTag("cone1");
				auto RC1 = object1->getComponent<ShapeRendererComponent>();
				auto object2 = ObjectsManager::findByTag("cone2");
				auto RC2 = object2->getComponent<ShapeRendererComponent>();
				auto object3 = ObjectsManager::findByTag("cone3");
				auto RC3 = object3->getComponent<ShapeRendererComponent>();
				auto object4 = ObjectsManager::findByTag("cone4");
				auto RC4 = object4->getComponent<ShapeRendererComponent>();
				
				RC1->renderEnable = RC2->renderEnable = RC3->renderEnable = RC4->renderEnable = isTreeNeedRender;
			}
		}

		if (ImGui::CollapsingHeader("Hyperboloid"))
		{
			auto hyperboloid = ObjectsManager::findByTag("hyperboloid");
			auto RC = hyperboloid->getComponent<HyperboloidRendererComponent>();

			if (ImGui::Checkbox("render##hyperboloid", &RC->renderEnable));

			if (ImGui::CollapsingHeader("Scale"))
			{
				auto TC = hyperboloid->getComponent<TransformComponent>();

				if (ImGui::SliderFloat("x##scale", &hyperboloidScale.x, 1, 5))
					TC->setSize(hyperboloidScale);
				if (ImGui::SliderFloat("y##scale", &hyperboloidScale.y, 1, 5))
					TC->setSize(hyperboloidScale);
				if (ImGui::SliderFloat("z##scale", &hyperboloidScale.z, 1, 5))
					TC->setSize(hyperboloidScale);
			}

			if (ImGui::CollapsingHeader("Rotation"))
			{
				auto TC = hyperboloid->getComponent<TransformComponent>();

				if (ImGui::SliderFloat("x##rotation", &hyperboloidRotation.x, 0, 360))
					TC->setRotation(hyperboloidRotation);
				if (ImGui::SliderFloat("y##rotation", &hyperboloidRotation.y, 0, 360))
					TC->setRotation(hyperboloidRotation);
				if (ImGui::SliderFloat("z##rotation", &hyperboloidRotation.z, 0, 360))
					TC->setRotation(hyperboloidRotation);
			}

			if (ImGui::SliderInt("layers##hp", &hyperboloidLayers, 2, 10))
			{
				auto hRC = hyperboloid->getComponent<HyperboloidRendererComponent>();
				hRC->setLayersCount(hyperboloidLayers);
			}

			if (ImGui::SliderInt("points##hp", &hyperboloidPoints, 3, 15))
			{
				auto hRC = hyperboloid->getComponent<HyperboloidRendererComponent>();
				hRC->setPointsPerLayerCount(hyperboloidPoints);
			}
		}
	}

/*--------------------LAB-4--------------------*/
	static void lab4() {}
	
	static void lab4_task1() {}

/*---------------------------------------------*/

	static void setCurrentLab(int value)
	{
		currentLab = value;
		currentExercise = 0;

		ObjectsManager::deleteGameObjects();
		ObjectsManager::loadGameObjects("resources\\labs\\" + std::to_string(currentLab) + ".json");

		if (currentLab == 3)
		{
			int indicesCount = 6;
			int vertexCount = 4;
			GLuint* indices = new GLuint[indicesCount];
			GLfloat* points = new GLfloat[vertexCount * 3];
			GLfloat* colors = new GLfloat[vertexCount * 3];

			// PRIMITIVES

			{
				auto polygon1 = ObjectsManager::findByTag("polygon1");
				auto RC1 = polygon1->getComponent<ShapeRendererComponent>();

				points[0] = 0.0f;	points[1] = 0.0f;	points[2] = 1.0f;
				points[3] = 0.2f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 0.8f;	points[7] = 1.0f;	points[8] = 1.0f;
				points[9] = 1.0f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				for (unsigned int i = 0; i < vertexCount; ++i)
				{
					colors[i * 3] = rand() % 255 / 255.f;
					colors[i * 3 + 1] = rand() % 255 / 255.f;
					colors[i * 3 + 2] = rand() % 255 / 255.f;
				}

				Primitive* polygon = new Primitive(indices, indicesCount, points, vertexCount * 3, colors, GL_TRIANGLES);
				delete RC1->primitive;
				RC1->primitive = polygon;
			}

			{
				auto polygon2 = ObjectsManager::findByTag("polygon2");
				auto RC2 = polygon2->getComponent<ShapeRendererComponent>();

				points[0] = 0.0f;	points[1] = 0.25f;	points[2] = 1.0f;
				points[3] = 0.0f;	points[4] = 0.75f;	points[5] = 1.0f;
				points[6] = 1.0f;	points[7] = 0.75f;	points[8] = 1.0f;
				points[9] = 1.0f;	points[10] = 0.25f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				for (unsigned int i = 0; i < vertexCount; ++i)
				{
					colors[i * 3] = rand() % 255 / 255.f;
					colors[i * 3 + 1] = rand() % 255 / 255.f;
					colors[i * 3 + 2] = rand() % 255 / 255.f;
				}

				Primitive* polygon = new Primitive(indices, indicesCount, points, vertexCount * 3, colors, GL_TRIANGLES);
				delete RC2->primitive;
				RC2->primitive = polygon;
			}

			{
				auto polygon3 = ObjectsManager::findByTag("polygon3");
				auto RC3 = polygon3->getComponent<ShapeRendererComponent>();

				points[0] = 0.25f;	points[1] = 0.5f;	points[2] = 1.0f;
				points[3] = 0.5f;	points[4] = 1.0f;	points[5] = 1.0f;
				points[6] = 0.75f;	points[7] = 0.5f;	points[8] = 1.0f;
				points[9] = 0.5f;	points[10] = 0.0f;	points[11] = 1.0f;

				indices[0] = 0; indices[1] = 1; indices[2] = 2;
				indices[3] = 2; indices[4] = 3; indices[5] = 0;

				for (unsigned int i = 0; i < vertexCount; ++i)
				{
					colors[i * 3] = rand() % 255 / 255.f;
					colors[i * 3 + 1] = rand() % 255 / 255.f;
					colors[i * 3 + 2] = rand() % 255 / 255.f;
				}

				Primitive* polygon = new Primitive(indices, indicesCount, points, vertexCount * 3, colors, GL_TRIANGLES);
				delete RC3->primitive;
				RC3->primitive = polygon;
			}

			delete[] indices;
			delete[] colors;
			delete[] points;

			// CUBE
			{
				auto cube = ObjectsManager::findByTag("cube");
				auto RC = cube->getComponent<ShapeRendererComponent>();
				delete RC->primitive;
				RC->primitive = getCube();
				RC->renderEnable = false;
			}

			// TREE
			{
				auto cone1 = ObjectsManager::findByTag("cone1");
				auto RC1 = cone1->getComponent<ShapeRendererComponent>();
				delete RC1->primitive;
				RC1->primitive = getCone();
				
				auto cone2 = ObjectsManager::findByTag("cone2");
				auto RC2 = cone2->getComponent<ShapeRendererComponent>();
				delete RC2->primitive;
				RC2->primitive = getCone();

				auto cone3 = ObjectsManager::findByTag("cone3");
				auto RC3 = cone3->getComponent<ShapeRendererComponent>();
				delete RC3->primitive;
				RC3->primitive = getCone(1);

				auto cone4 = ObjectsManager::findByTag("cone4");
				auto RC4 = cone4->getComponent<ShapeRendererComponent>();
				delete RC4->primitive;
				RC4->primitive = getCone(2);

				RC1->renderEnable = RC2->renderEnable = RC3->renderEnable = RC4->renderEnable = isTreeNeedRender;

			}

			// HYPERBOLOID
			{
				auto hyperboloid = ObjectsManager::findByTag("hyperboloid");
				auto RC = hyperboloid->getComponent<HyperboloidRendererComponent>();
				RC->renderEnable = false;
			}
		}
	}

	static void drawMenuStrip()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::MenuItem("Lab 2"))
			{
				setCurrentLab(2);
				lab2_task1();
			}

			if (ImGui::MenuItem("Lab 3"))
			{
				setCurrentLab(3);
			}

			if (ImGui::MenuItem("Lab 4"))
			{
				setCurrentLab(4);
				lab4_task1();
			}

			ImGui::EndMainMenuBar();
		}
	}

	static void drawRendererTools()
	{
		if (ImGui::CollapsingHeader("Renderer"))
		{
			if (ImGui::SliderInt("PointSize", &pointSize, 1, 10))
			{
				Renderer::setPointSize(pointSize);
			}

			if (ImGui::Combo("PolygonMode", &currentPolygonMode, polygonModes, 3))
			{
				switch (currentPolygonMode)
				{
				case 0: Renderer::setPolygonMode(GL_FILL); break;
				case 1: Renderer::setPolygonMode(GL_LINE); break;
				case 2: Renderer::setPolygonMode(GL_POINT); break;
				}
			}

			if (ImGui::Checkbox("Depth Test", &depthTest))
			{
				if (depthTest)
					Renderer::enableDepthTest();
				else
					Renderer::disableDepthTest();
			}
		}
	}

	static void drawCameraTools()
	{
		auto camera = ObjectsManager::findByTag("mainCamera");
		auto CC = camera->getComponent<CameraComponent>();
		

		if (ImGui::CollapsingHeader("Camera"))
		{
			if (ImGui::Checkbox("Perspective", &isCameraPerspective))
			{
				if (isCameraPerspective)
					CC->setProjectionMode(CameraComponent::ProjectionMode::Perspective);
				else
					CC->setProjectionMode(CameraComponent::ProjectionMode::Orthographic);
			}
		}
	}

public:
	static void init()
	{
		ResourceManager::loadResources("resources\\resources.json");

		ObjectsManager::loadGameObjectsSamples("resources\\objects.json");
		ObjectsManager::loadGameObjects("resources\\labs\\2.json");

		srand(Time::getTime());
		lab2_task1();
	}

	static void update()
	{
		UiHandler::onDrawBegin();

		drawMenuStrip();

		ImGui::SetNextWindowPos(ImVec2(0, 18));
		ImGui::SetNextWindowSize(ImVec2(225, Window::size.y - 18));
		
		if (ImGui::Begin(std::string("Lab " + std::to_string(currentLab)).c_str(), nullptr,
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
		{
			drawRendererTools();
			ImGui::NewLine();
			drawCameraTools();
			ImGui::NewLine();

			switch (currentLab)
			{
				case 2: lab2(); break;
				case 3: lab3(); break;
				case 4: lab4(); break;
			}
			ImGui::End();
		}

		UiHandler::onDrawEnd();
	}
};

unsigned int Lab::currentLab = 2;
const char* Lab::exercises[] = { "1", "2", "4" };
const char* Lab::drawModes[] = { "GL_TRIANGLES", "GL_TRIANGLE_FAN", "GL_LINE_LOOP" };
const char* Lab::figures[] = { "Triangle", "Trapezoid", "N-gon", "Parallelogram", "Rectangle", "Deltoid", "Rhombus" };
const char* Lab::polygonModes[] = {  "GL_FILL", "GL_LINE", "GL_POINT" };

bool Lab::depthTest = false;
bool Lab::isCameraPerspective = false;

int Lab::pointSize = 1;
int Lab::currentPolygonMode = 0;

int Lab::currentExercise = 0;
int Lab::currentDrawMode = 0;
int Lab::currentFigure = 0;
int Lab::anglesCount = 3;

bool Lab::isTreeNeedRender = false;
bool Lab::isFlatsNeedRender = true;

glm::vec3 Lab::hyperboloidRotation = glm::vec3(0.f, 0.f, 0.f);
glm::vec3 Lab::hyperboloidScale = glm::vec3(1.f, 1.f, 1.f);
int Lab::hyperboloidLayers = 5;
int Lab::hyperboloidPoints = 5;