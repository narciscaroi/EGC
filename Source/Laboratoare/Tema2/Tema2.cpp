#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
const float PI = 3.1415f;

using namespace std;
vector<int> platformColor;
vector<glm::vec3> colors;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	//camera
	renderCameraTarget = false;
	camera = new Tema::Camera();
	projectType = 0;

	//colors
	platformColor.push_back(0);
	platformColor.push_back(0);
	platformColor.push_back(0);
	colors.push_back(bluePlatform);
	colors.push_back(redPlatform);
	colors.push_back(yellowPlatform);
	colors.push_back(orangePlatform);
	colors.push_back(greenPlatform);

	//sphere
	sphereColor = glm::vec3(0.02f, 0.92f, 0.96f);
	sphereX = 0, sphereY = 0.5, sphereZ = 0, sphereMaxX = 1.5, sphereMaxY = 1, spherePos = 0;
	sphereJumped = false, sphereIsMoving = false;
	sphereGettingUp = false, sphereGettingDown = false, sphereGotDestroyed = false;
	platformDecelerateSpeed = 1, sphereCurrentPlatform = 1, sphereMode = 0, scaleFactor = 0.5f;

	//platform
	platformLength = 10;
	rndSpaceFBBETPlatforms = rand() % 3;
	platformX = 0, platformZ = 0;
	platformDecelerate = false, platformMoveForward = false;

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema2");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}
}

Mesh* Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	meshes[name] = new Mesh(name);
	meshes[name]->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	meshes[name]->vertices = vertices;
	meshes[name]->indices = indices;
	return meshes[name];
}


void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::GetSpherePos() {
	if (!sphereGotDestroyed) {
		if ((sphereX > platformX - spaceLRBetPlatforms - platformMidWidth)
			&& (sphereX < platformX - spaceLRBetPlatforms + platformMidWidth)) {
			spherePos = 0;
		}

		if ((sphereX > platformX - platformMidWidth) &&
			(sphereX < platformX + platformMidWidth)) {
			spherePos = 1;
		}

		if ((sphereX > platformX + spaceLRBetPlatforms - platformMidWidth)
			&& (sphereX < platformX + spaceLRBetPlatforms + platformMidWidth)) {
			spherePos = 2;
		}
	}
}

void Tema2::Update(float deltaTimeSeconds)
{
	if (sphereFuel <= 0) {
		cout << "Sphere run out of fuel. GAME OVER!" << endl;
		window->Close();
	}
	//first person view
	if (projectType == 1) {
		//cout << "DA" << endl;
		camera->Set(glm::vec3(sphereX, sphereY+1, 0), glm::vec3(sphereX, sphereY, -5), glm::vec3(0, 1, 0));
	}
	//third person view
	else {
		//cout << "NU" << endl;
		camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
	}
	if (sphereGotOnOrangePlat) {
		timeHighSpeedFrmOrg += 100 * deltaTimeSeconds;
		platformZ += maxSpeed * deltaTimeSeconds;
		if (timeHighSpeedFrmOrg == timeFromOrange) {
			timeHighSpeedFrmOrg = 0;
			sphereGotOnOrangePlat = false;
		}
	}
	else if (!sphereGotOnOrangePlat) {
		if (!sphereGotDestroyed && platformMoveForward) {
			platformZ += platformSpeed * deltaTimeSeconds;
		}
		else if (!sphereGotDestroyed && platformDecelerate) {
			platformZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
		}
	}
	GetSpherePos();
	if (!sphereGotDestroyed && sphereJumped == true) {
		if (sphereGettingUp) {
			sphereY += sphereJumpSpeed * deltaTimeSeconds;
		}
		else if (sphereGettingDown) {
			sphereY -= sphereJumpSpeed * deltaTimeSeconds;
		}

		if (sphereY >= 1) {
			sphereGettingUp = false;
			sphereGettingDown = true;
		}
		if (sphereY <= platformY + 0.5f) {
			sphereY = platformY + 0.5f;
			sphereGettingDown = false;
			sphereJumped = false;


			if (platformColor[spherePos] == 1) { //red
				cout << "You jumped on a red platfrom. GAME OVER" << endl;
				sphereGotDestroyed = true;
			}
			if (platformColor[spherePos] == 2) { //yellow
				cout << "You jumped to a yellow platform. You've lost some fuel" << endl;
				sphereFuel -= 5;

			}
			if (platformColor[spherePos] == 3) { //orange
				cout << "You jumped to an orange platform. You're frozen at a specific speed" << endl;
				sphereGotOnOrangePlat = true;

			}
			if (platformColor[spherePos] == 4) { //green
				cout << "You jumped to a green platform. You've got some fuel." << endl;
				sphereFuel += 10;
			}
			platformColor[spherePos] = rand() % 5;
		}
	}


	{
		if (sphereGotDestroyed == 1) {
			scaleFactor += 40 * deltaTimeSeconds;
			if (scaleFactor >= 20) {
				cout << "GAME OVER!" << endl;
				window->Close();
			}
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix, sphereColor);

	}
	//platform
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX - spaceLRBetPlatforms, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, colors[platformColor[0]]);
	
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, colors[platformColor[1]]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX + spaceLRBetPlatforms, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, colors[platformColor[2]]);
	}

	//fuel
	sphereFuel -= 2 * deltaTimeSeconds;
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(-1.7f, 2.2f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(sphereFuel/100, 0.02f, 0.01f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema2"], modelMatrix, colors[4]);
	}

	if (sphereGotDestroyed) {
		if (sphereY > -2) {
			sphereY -= sphereJumpSpeed * deltaTimeSeconds;
		}
		else {
			sphereMode = 1;
		}

	}
	else if (!sphereGotDestroyed) {
		if (sphereIsMoving && (sphereZ > ((platformLength / 2) - platformZ))) {
			platformLength = rand() % 100 + minPlatLength;
			rndSpaceFBBETPlatforms = rand() % 2 + spaceFBBetPlatforms;
			platformZ = -(platformLength / 2) - rndSpaceFBBETPlatforms;
			platformColor[0] = rand() % 5;
			platformColor[1] = rand() % 5;
			platformColor[2] = rand() % 5;
			if (!sphereJumped) {
				sphereGotDestroyed = true;
			}
		}

		//sphera & plat LR
		if (((sphereX > platformX - spaceLRBetPlatforms + platformMidWidth) && (sphereX < platformX - platformMidWidth)) ||
			((sphereX > platformX + platformMidWidth) && (sphereX < platformX + spaceLRBetPlatforms - platformMidWidth)) ||
			(sphereX < platformX - spaceLRBetPlatforms - platformMidWidth) ||
			(sphereX > platformX + spaceLRBetPlatforms + platformMidWidth)) {
			if (!sphereJumped) {
				sphereGotDestroyed = true;
			}
		}
	}
}

void Tema2::FrameEnd()
{
	DrawCoordinatSystem();
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colorSet)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);

	// TODO : get shader location for uniform mat4 "Model"
	int locationModel = glGetUniformLocation(shader->program, "Model");
	// TODO : set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(locationModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	// TODO : get shader location for uniform mat4 "View"
	int locationView = glGetUniformLocation(shader->program, "View");
	// TODO : set shader uniform "View" to viewMatrix
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(locationView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	// TODO : get shader location for uniform mat4 "Projection"
	int locationProjection = glGetUniformLocation(shader->program, "Projection");

	// TODO : set shader uniform "Projection" to projectionMatrix
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(locationProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	
	int clr = glGetUniformLocation(shader->program, "colorSet");
	glUniform3fv(clr, 1, glm::value_ptr(colorSet));

	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	// add key press event
	if (window->KeyHold(GLFW_KEY_SPACE) && !sphereJumped) {
		sphereJumped = true;
		sphereGettingUp = true;
	}
	if (window->KeyHold(GLFW_KEY_W)) {
		platformMoveForward = true;
		platformDecelerateSpeed = 1;
		sphereIsMoving = true;
	}
	else if (window->KeyHold(GLFW_KEY_S)) {
		platformDecelerate = true;
		platformMoveForward = false;
		if (platformDecelerateSpeed >= 0) {
			platformDecelerateSpeed -= 2 *  deltaTime;
		}
		if (platformDecelerateSpeed < 0) {
			platformDecelerateSpeed = 0;
			sphereIsMoving = false;
		}
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		sphereX -= sphereLRSpeed * deltaTime;
	}
	else if (window->KeyHold(GLFW_KEY_D)) {
		sphereX += sphereLRSpeed * deltaTime;
	}
	
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_F) {
		projectType = (projectType + 1) % 2;
	}
}

void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_S) {
		platformDecelerate = false;
	}
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
