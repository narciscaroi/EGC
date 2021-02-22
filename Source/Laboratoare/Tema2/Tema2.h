#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include "TemaCamera.h"

class Tema2 : public SimpleScene
{
public:
	Tema2();
	~Tema2();

	void Init() override;

	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

protected:
	//camera
	Tema::Camera* camera;
	glm::mat4 projectionMatrix;
	bool renderCameraTarget;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colorSet);

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	void GetSpherePos();
	Mesh* CreateSphere(const char* name, float radius);

	float elapsedTime;
	glm::vec3 color;

	int projectType; //1-first person, 0-third

	//sphere
	float const sphereJumpSpeed = 1, sphereLRSpeed = 2, spX = 0, spY = 0.5f, timeFromOrange = 3, maxSpeed = 25;
	float sphereX, sphereY, sphereZ, sphereMaxX, sphereMaxY, scaleFactor;
	int spherePos; //0 stanga, 1 centru, 2 dreapta
	bool sphereJumped, sphereMovedLeft, sphereMovedRight, sphereIsMoving, sphereGettingUp, sphereGettingDown;
	bool sphereGotDestroyed, sphereGotOnOrangePlat;
	float sphereCurrentPlatform, timeHighSpeedFrmOrg, sphereFuel = 100;
	int sphereMode;
	glm::vec3 sphereColor;

	//platforme
	float const platformSpeed = 10, spaceLRBetPlatforms = 2, spaceFBBetPlatforms = 3, minPlatLength = 5; 
	float platformLength, rndSpaceFBBETPlatforms, platformMidWidth = 0.5f;
	float platformX, platformY, platformZ;
	float platformDecelerateSpeed, platRndY;
	bool platformMoveForward, platformDecelerate;
	glm::vec3 bluePlatform = glm::vec3(0.12f, 0.2f, 0.9f);
	glm::vec3 redPlatform = glm::vec3(0.9f, 0.16f, 0.04f);
	glm::vec3 yellowPlatform = glm::vec3(0.96f, 0.97f, 0.04f);
	glm::vec3 orangePlatform = glm::vec3(1, 0.6f, 0.04f);
	glm::vec3 greenPlatform = glm::vec3(0, 0.92f, 0.05f);
};
