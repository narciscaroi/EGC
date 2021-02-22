#include "Laborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

int ColorChangeCounter = 0;
int GKeyCounter = 0;
float x = 0;
float y = 0;
float z = 0;
int IsWPressed = 0;
int IsSPressed = 0;
int IsDPressed = 0;
int IsAPressed = 0;
int IsQPressed = 0;
int IsEPressed = 0;
int IsCTRLPressed = 0;
float speed = 0.1f;

// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

Laborator1::Laborator1()
{
}

Laborator1::~Laborator1()
{
}

void Laborator1::Init()
{
	// Load a mesh from file into GPU memory
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
		
		Mesh* mesh2 = new Mesh("switcher");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
	}
}

void Laborator1::FrameStart()
{

}

void Laborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	if (ColorChangeCounter == 1) {
		glClearColor(0.1f, 0.3f, 0.1f, 0);
	}
	else if (ColorChangeCounter == 0) {
		glClearColor(0, 0, 0, 1);
	}

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

	// render the object
	RenderMesh(meshes["box"], glm::vec3(1, 0.5f, 0), glm::vec3(0.5f));

	// render the object again but with different properties
	RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0));
	RenderMesh(meshes["switcher"], glm::vec3(x, y, z));

}

void Laborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Laborator1::OnInputUpdate(float deltaTime, int mods)
{
	// treat continuous update based on input
	if (IsWPressed == 1) {
		z -= speed;
		if (IsCTRLPressed == 1) {
			IsWPressed = 0;
			IsCTRLPressed = 0;
		}
	}
	else if (IsSPressed == 1) {
		z += speed;
		if (IsCTRLPressed == 1) {
			IsSPressed = 0;
			IsCTRLPressed = 0;
		}
	}
	else if (IsDPressed) {
		x += speed;
		if (IsCTRLPressed == 1) {
			IsDPressed = 0;
			IsCTRLPressed = 0;
		}
	}
	else if (IsAPressed) {
		x -= speed;
		if (IsCTRLPressed == 1) {
			IsAPressed = 0;
			IsCTRLPressed = 0;
		}
	}
	else if (IsQPressed) {
		y += speed;
		if (IsCTRLPressed == 1) {
			IsQPressed = 0;
			IsCTRLPressed = 0;
		}
	}
	else if (IsEPressed) {
		y -= speed;
		if (IsCTRLPressed == 1) {
			IsEPressed = 0;
			IsCTRLPressed = 0;
		}
	}
};

void Laborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		ColorChangeCounter = 1;
	}
	if (key == GLFW_KEY_G) {
		Mesh* mesh = mesh = new Mesh("switcher");
		if (GKeyCounter == 0) {
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
			GKeyCounter = 1;
		}
		else if (GKeyCounter == 1) {
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Props", "oildrum.obj");
			GKeyCounter = 2;
		}
		else if (GKeyCounter == 2) {
			mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
			GKeyCounter = 0;
		}
		meshes[mesh->GetMeshID()] = mesh;
	}
	if (mods == GLFW_MOD_CONTROL) {
		IsCTRLPressed = 1;
	}
	if(key == GLFW_KEY_W) {
		IsWPressed = 1;
	}
	else if(key == GLFW_KEY_S) {
		IsSPressed = 1;
	}
	else if(key == GLFW_KEY_D) {
		IsDPressed = 1;
	}
	else if (key == GLFW_KEY_A) {
		IsAPressed = 1;
	}
	else if (key == GLFW_KEY_Q) {
		IsQPressed = 1;
	}
	else if (key == GLFW_KEY_E) {
		IsEPressed = 1;
	}
};

void Laborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_F) {
		ColorChangeCounter = 0;
	}
	if (key == GLFW_KEY_W) {
		IsWPressed = 0;
	}
	else if (key == GLFW_KEY_S) {
		IsSPressed = 0;
	}
	else if (key == GLFW_KEY_D) {
		IsDPressed = 0;
	}
	else if (key == GLFW_KEY_A) {
		IsAPressed = 0;
	}
	else if (key == GLFW_KEY_Q) {
		IsQPressed = 0;
	}
	else if (key == GLFW_KEY_E) {
		IsEPressed = 0;
	}
};

void Laborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void Laborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void Laborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
	// treat mouse scroll event
}

void Laborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
