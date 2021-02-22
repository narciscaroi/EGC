#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>
#include <Engine\Component\Transform\Transform.h>

class Tema3 : public SimpleScene
{
public:
	Tema3();
	~Tema3();

	void Init() override;

	Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned short>& indices);

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 colorSet, Texture2D* texture = NULL, int isSphere = 0);

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

	float score;

	//textures
	std::unordered_map<std::string, Texture2D*> mapTextures;

	//sphere
	float const sphereJumpSpeed = 1, sphereLRSpeed = 2, spX = 0, spY = 0.5f, timeFromOrange = 3, maxSpeed = 25;
	float sphereX, sphereY, sphereZ, sphereMaxX, sphereMaxY, scaleFactor;
	int spherePos; //0 stanga, 1 centru, 2 dreapta
	bool sphereJumped, sphereMovedLeft, sphereMovedRight, sphereIsMoving, sphereGettingUp, sphereGettingDown;
	bool sphereGotDestroyed = false, sphereCrashed = false;
	float sphereCurrentPlatform, timeHighSpeedFrmOrg;
	int sphereMode;
	glm::vec3 sphereColor;

	//platforme
	float const platformSpeed = 10, spaceLRBetPlatforms = 2, spaceFBBetPlatforms = 3, minPlatLength = 5;
	float platformLength, rndSpaceFBBETPlatforms, platformMidWidth = 0.5f;
	float platformX, platformY, platformZ;
	float platformDecelerateSpeed, platRndY;
	bool platformMoveForward, platformDecelerate;
	glm::vec3 platColor = glm::vec3(0.12f, 0.2f, 0.9f);

	//colectabile
	float collectableZ, collectableX, collectableY;
	int collectableRndZ, collectRndPos;
	bool collectableWasTaken;
	float collectableTaken = 0;

	//magnet
	float magnetX, magnetY, magnetZ = 100, magnetRndZ, magnetRange = 4.5f;
	float magnetAttractionSpeed = 10;
	int magnetRndPos;
	bool magnetWasTaken, collectableIsAttract = false;
	//coin
	float coinX, coinY, coinZ = 100;
	int coinRndZ, coindRndPos;
	bool coinWasTaken = false;
	float coinsTaken = 0;

	//wall
	float wallZ = 100, wallY;

	//decoratiuni
	float angluarRotate = 0;
	float sunX = -7, sunY=6, sunZ=-22;

	//iluminare
	float materialShininess = 50, materialKd = 0.5f, materialKa = 0.5f, materialKs = 0.5f;
};
