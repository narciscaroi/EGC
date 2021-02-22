#include "Tema3.h"

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#include <Core/Engine.h>
const float PI = 3.1415f;

using namespace std;

vector<float> jmpCoinsX;
vector<float> jmpColX;

vector<float> jmpCoinsY;
vector<float> jmpColY;

vector<float> jmpCoinsZ;
vector<float> jmpColZ;
vector<float> rndPickLRBCol;
vector<float> rndPickLRBCoins;
vector<int> colArrivedUp;
vector<int> coinArrivedUp;
bool jmpWasPicked = false;


Tema3::Tema3()
{
}

Tema3::~Tema3()
{
}

void Tema3::Init()
{
	const string textureLoc = "Source/Laboratoare/Tema3/Textures/";
	// Load textures
	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "da.jpg").c_str(), GL_REPEAT);
		mapTextures["p1"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "ssd.jpg").c_str(), GL_REPEAT);
		mapTextures["p2"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "da.jpg").c_str(), GL_REPEAT);
		mapTextures["p3"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "Greeble_Techno.jpg").c_str(), GL_REPEAT);
		mapTextures["sph"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "old.jpg").c_str(), GL_REPEAT);
		mapTextures["old"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "books.jpg").c_str(), GL_REPEAT);
		mapTextures["books"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "wall.jpg").c_str(), GL_REPEAT);
		mapTextures["wall"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "mars.jpg").c_str(), GL_REPEAT);
		mapTextures["mars"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "venus.jpg").c_str(), GL_REPEAT);
		mapTextures["venus"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "jupiter.jpg").c_str(), GL_REPEAT);
		mapTextures["jupiter"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "neptune.jpg").c_str(), GL_REPEAT);
		mapTextures["neptune"] = texture;
	}


	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "sun.jpg").c_str(), GL_REPEAT);
		mapTextures["sun"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "stars.jpg").c_str(), GL_REPEAT);
		mapTextures["sky"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "stars_milkyway.jpg").c_str(), GL_REPEAT);
		mapTextures["skyy"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "magnet.png").c_str(), GL_REPEAT);
		mapTextures["magnet"] = texture;
	}

	{
		Texture2D* texture = new Texture2D();
		texture->Load2D((textureLoc + "gold.jpg").c_str(), GL_REPEAT);
		mapTextures["coin"] = texture;
	}


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

	//collectable
	collectableWasTaken = false;
	collectableZ = 100;
	elapsedTime = 0;

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

	{
		Mesh* mesh = new Mesh("sq");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "screen_quad.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("coin");
		mesh->LoadMesh("Source/Laboratoare/Tema3", "coin.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}


	// Create a shader program for drawing face polygon with the color of the normal
	{
		Shader* shader = new Shader("ShaderTema3");
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema3/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		{
			Shader* shader = new Shader("ShaderSkyBox");
			shader->AddShader("Source/Laboratoare/Tema3/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
			shader->AddShader("Source/Laboratoare/Tema3/Shaders/SkyBoxShader.glsl", GL_FRAGMENT_SHADER);
			shader->CreateAndLink();
			shaders[shader->GetName()] = shader;
		}
	}
}

Mesh* Tema3::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices)
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


void Tema3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::GetSpherePos() {
	if (!sphereGotDestroyed && !sphereCrashed) {
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

void Tema3::Update(float deltaTimeSeconds)
{

	if (!sphereGotDestroyed && !sphereCrashed && platformMoveForward) {
		platformZ += platformSpeed * deltaTimeSeconds;
		collectableZ += platformSpeed * deltaTimeSeconds;
		wallZ += platformSpeed * deltaTimeSeconds;
		magnetZ += platformSpeed * deltaTimeSeconds;
		coinZ += platformSpeed * deltaTimeSeconds;
	}
	else if (!sphereGotDestroyed && !sphereCrashed && platformDecelerate) {
		platformZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
		collectableZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
		wallZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
		magnetZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
		coinZ += platformSpeed * deltaTimeSeconds * platformDecelerateSpeed;
	}

	//colect by crash them
	if (collectableWasTaken == true) {
		collectableY += 10 * deltaTimeSeconds;
	}
	if (coinWasTaken == true) {
		coinY += 10 * deltaTimeSeconds;
	}

	if (magnetWasTaken == true) {
		collectableIsAttract = true;
		elapsedTime = 0;
		magnetWasTaken = false;
	}

	//colect with magnet
	if (collectableIsAttract == true && glm::distance(sphereZ, collectableZ) < magnetRange) {
		elapsedTime += 10 * deltaTimeSeconds;
		if (sphereZ >= collectableZ) {
			collectableX -= (collectableX - sphereX)/5;
			collectableY -= (collectableY - sphereY)/5;
			collectableZ -= (collectableZ - sphereZ)/5;
		}

		if (sphereZ >= coinZ) {
			coinX -= (coinX - sphereX) / 5;
			coinY -= (coinY - sphereY) / 5;
			coinZ -= (coinZ - sphereZ) / 5;
		}

		if (elapsedTime >= 15) {
			collectableIsAttract = false;
		}
		
	}

	//sphere jumped out of platforms
	if (sphereGotDestroyed && !sphereCrashed) {
		if (sphereY > -2) {
			sphereY -= sphereJumpSpeed * deltaTimeSeconds;
		}
		else {
			sphereMode = 1;
		}

	}
	else if (!sphereGotDestroyed && !sphereCrashed) {
		if (sphereIsMoving && (sphereZ > ((platformLength / 2) - platformZ))) {
			platformLength = rand() % 100 + minPlatLength;
			rndSpaceFBBETPlatforms = rand() % 2 + spaceFBBetPlatforms;
			platformZ = -(platformLength / 2) - rndSpaceFBBETPlatforms;
			if (!sphereJumped) {
				sphereGotDestroyed = true;
			}

			//wall
			if (platformLength > 8) {
				wallZ = platformZ;
			}
			wallY = platformY + 0.5f;

			//collectable
			collectableY = platformY;
			collectableRndZ = rand() % 5;
			if ((int)(100 * deltaTimeSeconds) % 2 == 0) {
				collectableZ = -(platformLength / 2) + collectableRndZ;
			}
			else {
				collectableZ = -(platformLength / 2) - collectableRndZ;
			}
			collectableWasTaken = false;
			collectRndPos = rand() % 3;
			if (collectRndPos == 0) {
				collectableX = platformX - spaceLRBetPlatforms;
				coindRndPos = rand() % 2 + 1;
				if (coindRndPos == 1) {
					coinX = platformX;
				}
				else {
					coinX = platformX + spaceLRBetPlatforms;
				}
			}
			else if (collectRndPos == 1) {
				collectableX = platformX;
				coindRndPos = (rand() % 2) * 2;

			}
			else {
				collectableX = platformX + spaceLRBetPlatforms;
				coindRndPos = rand() % 2;
			}

			if (coindRndPos == 0) {
				coinX = platformX + spaceLRBetPlatforms;
			}
			else if (coindRndPos == 1) {
				coinX = platformX;
			}
			else {
				coinX = platformX + spaceLRBetPlatforms;
			}

			//magnet
			magnetY = platformY;
			if (platformLength <= 10) {
				magnetZ = (int)platformLength % 3;
			}
			else {
				magnetRndZ = rand() % 2 + ((int)platformLength) % 5;
				if ((int)(100 * deltaTimeSeconds) % 2 == 0) {
					magnetZ = -(platformLength / 2) - magnetRndZ;
				}
				else {
					magnetZ = -(platformLength / 2) + magnetRndZ;
				}
			}

			magnetWasTaken = false;
			magnetRndPos = rand() % 3;
			if (magnetRndPos == 0) {
				magnetX = platformX - spaceLRBetPlatforms;
			}
			else if (magnetRndPos == 1) {
				magnetX = platformX;
			}
			else {
				magnetX = platformX + spaceLRBetPlatforms;
			}

			//coin
			coinY = 0.5f;
			if (platformLength < 10) {
				coinZ = (int)platformLength % 3;
			}
			else {
				coinRndZ = rand() % 2 + ((int)platformLength % 5 + 2);
				if ((int)(100 * deltaTimeSeconds) % 2 == 0) {
					coinZ = -(platformLength / 2) - coinRndZ;
				}
				else {
					coinZ = -(platformLength / 2) + coinRndZ;
				}
			}
			coinWasTaken = false;
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

		//wall
		if ((sphereZ < (wallZ + 0.1f)) && (sphereZ > (wallZ - 0.1f)) && !sphereJumped && ((wallY + 0.5f) > (sphereY - 0.5f))) {
			sphereCrashed = true;
		}

		GetSpherePos();
		//colectabile
		if ((spherePos == collectRndPos) && (sphereZ < (collectableZ + 0.1f) && sphereZ >(collectableZ - 0.1f))) {
			collectableWasTaken = true;
			score += 1;
			collectableTaken += 1;
		}
		if ((spherePos == magnetRndPos) && (sphereZ < (magnetZ + 0.1f) && sphereZ >(magnetZ - 0.1f))) {
			magnetWasTaken = true;
		}
		if ((spherePos == coindRndPos) && (sphereZ < (coinZ + 0.1f) && sphereZ >(coinZ - 0.1f))) {
			coinWasTaken = true;
			score += 2;
			coinsTaken += 1;
		}
	}


	GetSpherePos();
	if (!sphereGotDestroyed && !sphereCrashed && sphereJumped == true) {
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
		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Render Objects~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	{
		if (sphereGotDestroyed == 1) {
			scaleFactor += 20 * deltaTimeSeconds;
			if (scaleFactor >= 20) {
				cout << "GAME OVER!" << endl;
				cout << "SCORE: " << score << endl;
				window->Close();
			}
		}
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereX, sphereY, sphereZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["sph"], 1);

	}
	//platform
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX - spaceLRBetPlatforms, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["p1"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["p2"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX + spaceLRBetPlatforms, platformY, platformZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 0.5f, platformLength));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["p3"]);
	}

	//wall
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX - spaceLRBetPlatforms, platformY + 0.5f, wallZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.2f, 1));
		RenderSimpleMesh(meshes["sq"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["wall"]);

		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX, platformY + 0.5f, wallZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.2f, 1));
		RenderSimpleMesh(meshes["sq"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["wall"]);

	    modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(platformX + spaceLRBetPlatforms, platformY + 0.5f, wallZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 0.2f, 1));
		RenderSimpleMesh(meshes["sq"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["wall"]);

		
	}

	//magnet
	if (!magnetWasTaken) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(magnetX, magnetY + 0.5f, magnetZ));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.2f, 0.3f));
			RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["magnet"]);

		}
	}
	//colectabile
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(collectableX, collectableY + 0.5f, collectableZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.2f, 0.3f));
		RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["old"]);
	}
	//coin
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(coinX, coinY, coinZ));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.01f));
		modelMatrix = glm::rotate(modelMatrix, 3.14f / 2, glm::vec3(1, 0, 0));
		RenderSimpleMesh(meshes["coin"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["coin"]);
	}
	if (sphereCrashed) {
		elapsedTime += 10 * deltaTimeSeconds;
		if (!jmpWasPicked) {
			jmpWasPicked = true;
			for (int i = 0; i < collectableTaken; i += 1) {
				rndPickLRBCol.push_back(rand() % 3); //0 stanga, 1 dreapta, 2 fata
				jmpColX.push_back(sphereX);
				jmpColY.push_back(sphereY + 0.5f);
				jmpColZ.push_back(sphereZ);
				colArrivedUp.push_back(0);
			}
			for (int i = 0; i < coinsTaken; i+=1) {
				rndPickLRBCoins.push_back(rand() % 3);
				jmpCoinsX.push_back(sphereX);
				jmpCoinsY.push_back(sphereY + 0.5f);
				jmpCoinsZ.push_back(sphereZ);
				coinArrivedUp.push_back(0);
			}
			elapsedTime = 0;
		}
		//colectabile
		for (int i = 0; i < collectableTaken; i++) {
			if (rndPickLRBCol[i] == 0) { //stanga
				jmpColX[i] -= deltaTimeSeconds - ((rand()%2) /30);
			}
			else if (rndPickLRBCol[i] == 1) { //dreapta
				jmpColX[i] += deltaTimeSeconds + ((rand() % 2) / 30);
			}
			else if (rndPickLRBCol[i] == 2) { //fata
				jmpColZ[i] += deltaTimeSeconds;
			}

			if (jmpColY[i] <= 1.5f && colArrivedUp[i] == 0) {
				jmpColY[i] += deltaTimeSeconds + ((rand() % 2) / 30);
			}
			else {
				jmpColY[i] -= deltaTimeSeconds - ((rand() % 2) / 30);
				colArrivedUp[i] = 1;
			}
			//colectabile
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(jmpColX[i], jmpColY[i], jmpColZ[i]));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.2f, 0.3f));
				RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["old"]);

			}

		}
		//coin
		for (int i = 0; i < coinsTaken; i++) {
			if (rndPickLRBCoins[i] == 0) { //stanga
				jmpCoinsX[i] -= deltaTimeSeconds - ((rand() % 2) / 30);
			}
			else if (rndPickLRBCoins[i] == 1) { //dreapta
				jmpCoinsX[i] +=deltaTimeSeconds + ((rand() % 2) / 30);
			}
			else if (rndPickLRBCoins[i] == 2) { //fata
				jmpCoinsZ[i] +=deltaTimeSeconds + ((rand() % 2) / 30);
			}
			if (jmpCoinsY[i] <= 1.5f && coinArrivedUp[i] == 0) {
				jmpCoinsY[i] += deltaTimeSeconds + ((rand() % 2) / 30);
			}
			else {
				jmpCoinsY[i] -= deltaTimeSeconds - ((rand() % 2) / 30);
				coinArrivedUp[i] = 1;
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(jmpCoinsX[i], jmpCoinsY[i], jmpCoinsZ[i]));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 0.1f, 0.01f));
				modelMatrix = glm::rotate(modelMatrix, 3.14f / 2, glm::vec3(1, 0, 0));
				RenderSimpleMesh(meshes["coin"], shaders["ShaderTema3"], modelMatrix, platColor, mapTextures["coin"]);
			}
			
		}
		if (elapsedTime > 50) {
			sphereGotDestroyed = true;
			sphereCrashed = true;
		}
	}

	//decoratiuni
	{

		//sun
		angluarRotate += deltaTimeSeconds;
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(sunX, sunY, sunZ));
		modelMatrix = glm::rotate(modelMatrix, angluarRotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["sun"]);
		
		//mars
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(6, 4, -17));
		modelMatrix = glm::rotate(modelMatrix, angluarRotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["neptune"]);

		//venus
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(7, 3, -14));
		modelMatrix = glm::rotate(modelMatrix, angluarRotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["venus"]);

		//jupiter
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(8, 2, -11));
		modelMatrix = glm::rotate(modelMatrix, angluarRotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["jupiter"]);

		//neptune
		modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(9, 0.5f, -8));
		modelMatrix = glm::rotate(modelMatrix, angluarRotate, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, sphereColor, mapTextures["mars"]);

	}

	//scorebar
	if (score > 0) {
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(2.2f, 2.2f, 0));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(score / 100, 0.02f, 0.01f));
			RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, glm::vec3(0, 0.92f, 0.05f), NULL);

		}
	}

	//skybox
	{
		glm::mat4 modelMatrix = glm::mat4(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(50));
		modelMatrix = glm::rotate(modelMatrix, 3.14f, glm::vec3(0, 1, 0));
		RenderSimpleMesh(meshes["sphere"], shaders["ShaderSkyBox"], modelMatrix, sphereColor, mapTextures["skyy"]);
	}

}

void Tema3::FrameEnd()
{
	//DrawCoordinatSystem();
}

void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colorSet, Texture2D* texture, int isSphere)
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
	
	//texture 
	int has_texture = 0;
	if (texture)
	{
		//TODO : activate texture location 0
		glActiveTexture(GL_TEXTURE0);
		//TODO : Bind the texture1 ID
		glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
		//TODO : Send texture uniform value
		glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
		has_texture = 1;
	}

	int textureLocation = glGetUniformLocation(shader->program, "has_texture");
	glUniform1i(textureLocation, has_texture);

	int isSphereLocation = glGetUniformLocation(shader->program, "isSphere");
	glUniform1i(isSphereLocation, isSphere);

	//iluminare
	int punctiform_light_position = glGetUniformLocation(shader->program, "punctiform_light_position");
	glUniform3f(punctiform_light_position, sunX, sunY, sunZ);

	int spot_light_position = glGetUniformLocation(shader->program, "spot_light_position");
	glUniform3f(spot_light_position, sphereX, sphereY, sphereZ);

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	int eye_position = glGetUniformLocation(shader->program, "eye_position");
	glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

	int material_kd = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(material_kd, materialKd);

	int material_ka = glGetUniformLocation(shader->program, "material_ka");
	glUniform1f(material_ka, materialKa);

	int material_ks = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(material_ks, materialKs);

	int material_shininess = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(material_shininess, materialShininess);
	
	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void Tema3::OnInputUpdate(float deltaTime, int mods)
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
			platformDecelerateSpeed -= 2 * deltaTime;
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

void Tema3::OnKeyPress(int key, int mods)
{
	// add key press event
}

void Tema3::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_S) {
		platformDecelerate = false;
	}
}

void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema3::OnWindowResize(int width, int height)
{
}
