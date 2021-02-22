#include "Tema1.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2d.h"
#include <Laboratoare\tema1\Objects2D.h>
#include <time.h>
#include <string.h>
#include <math.h>

using namespace std;
bool wIsPressed, sIsPressed;
float timeSpent;
GLfloat posx = 10.5f;
GLfloat posy = 12.5f;
glm::vec3 color = glm::vec3(0.05f, 0.98f, 0.02f);

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	resx = resolution.x;
	resy = resolution.y;

	glm::vec3 corner = glm::vec3(0, 0, 0);
	// initialize tx and ty (the translation steps)
	translateX = 0;
	translateY = 0;

	// initialize sx and sy (the scale factors)
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;
	mouseButtonIsPressed = false;
	finished = false;
	lifes = 3;
	wIsPressed = false, sIsPressed = false;
	srand(time(0));
	score = 0;
	timeSpent = 0;

	//bow
	bowx = 75, bowy = 150,
	bow_radius = 70;
	//powerbar
	areaPBScale = 1;
	scoreBarX = 100, scorebarY = 690;
	//arrow
	arrowLength = 120;
	arrowSide = 5;
	arrow_pointer = 0;
	arrowThrounX = bowx,arrowThrounY = bowy;
	arrowSpeed = 0;
	arrowThrown = false;

	//ballons
	balloon_radiusX = 30;
	balloon_radiusY = 40;
	ballonsNumber = 5;
	for (int i = 0; i < ballonsNumber; i++) {
		ballonsColor[i] = rand() % 2;
		balloonsX[i] = 400 + 100 * i;
		balloonsY[i] = 10;
		balloonIsHit[i] = 0;
		balloonsSpeed[i] = (rand() % 6) + 1;
	}

	//shurikens
	radius_shuriken = 25, cy = 10;
	shurikenRotation = 0;
	for (int i = 0; i < shurikenNumber; i++) {
		shurikensX[i] = resx;
		shurikensY[i] = 100 + i * 50;
		balloonIsHit[i] = 0;
		shurikensSpeed[i] = (rand() % 9) + 1;
	}

	//randomObj
	randomObject = rand() % 3;
	randomObjRadius = 30;
	randomObjectX = 0;
	randomObjectY = 0;
	timeSpentForRndObj = 0;
	randomObjAng = atan2(resy, resx);
	randomObjWasHit = false;

	Mesh* bow = Objects2D::CreateBow("bow", 0, 0, bow_radius);
	AddMeshToList(bow);

	Mesh* perimPB = Objects2D::CreatePerimeterPowerBar("perimPB", bow_radius, arrowSide);
	AddMeshToList(perimPB);

	Mesh* areaPB = Objects2D::CreateAreaPowerBar("areaPB", 0.1f, arrowSide, 0);
	AddMeshToList(areaPB);

	Mesh* arrow = Objects2D::CreateArrow("arrow", corner, arrowLength, arrowSide);
	AddMeshToList(arrow);

	Mesh* redBalloon = Objects2D::CreateBalloon("redBalloon", balloon_radiusX, balloon_radiusY, 0, 0, 0);
	AddMeshToList(redBalloon);

	Mesh* yellowBalloon = Objects2D::CreateBalloon("yellowBalloon", balloon_radiusX, balloon_radiusY, 0, 0, 1);
	AddMeshToList(yellowBalloon);

	Mesh* heart = Objects2D::CreateHeart("heart");
	AddMeshToList(heart);

	Mesh* shuriken = Objects2D::CreateShuriken("shuriken", radius_shuriken, cy);
	AddMeshToList(shuriken);
	
	//parti balon
	Mesh* leftPartBalloonRed = Objects2D::CreateLeftPartBallon("leftPartBalloonRed", balloon_radiusX, balloon_radiusY, 0, 0, 0);
	AddMeshToList(leftPartBalloonRed);
	Mesh* rightPartBalloonRed = Objects2D::CreateRightPartBallon("rightPartBalloonRed", balloon_radiusX, balloon_radiusY, 0, 0, 0);
	AddMeshToList(rightPartBalloonRed);

	Mesh* leftPartBalloonYellow = Objects2D::CreateLeftPartBallon("leftPartBalloonYellow", balloon_radiusX, balloon_radiusY, 0, 0, 1);
	AddMeshToList(leftPartBalloonYellow);
	Mesh* rightPartBalloonYellow = Objects2D::CreateRightPartBallon("rightPartBalloonYellow", balloon_radiusX, balloon_radiusY, 0, 0, 1);
	AddMeshToList(rightPartBalloonYellow);

	Mesh* humanBody = Objects2D::CreateHumanBody("humanBody", 0, 0);
	AddMeshToList(humanBody);

	Mesh* posScoreBar = Objects2D::CreateAreaPowerBar("posScoreBar", 1, 2 * arrowSide, 0);
	AddMeshToList(posScoreBar);

	Mesh* negScoreBar = Objects2D::CreateAreaPowerBar("negScoreBar", 1, 2 * arrowSide, 1);
	AddMeshToList(negScoreBar);

	Mesh* ball1 = Objects2D::CreateBall1("ball1", 0, 0, randomObjRadius);
	AddMeshToList(ball1);
	Mesh* ball2 = Objects2D::CreateBall2("ball2", 0, 0, randomObjRadius);
	AddMeshToList(ball2);
	Mesh* ball3 = Objects2D::CreateBall3("ball3", 0, 0, randomObjRadius);
	AddMeshToList(ball3);

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.2f, 0, 0.5f, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
	if (finished) {
		cout << "SCORE: " << score << endl;
		cout << "GAME OVER" << endl;
		exit(0);
	}
	if (wIsPressed) {
		bowy += 250 * deltaTimeSeconds;
		if (!arrowThrown) {
			arrowThrounY += 250 * deltaTimeSeconds;
			arrowPeakY += 250 * deltaTimeSeconds;
		}
	}
	if (sIsPressed) {
		bowy -= 250 * deltaTimeSeconds;
		if (!arrowThrown) {
			arrowThrounY -= 250 * deltaTimeSeconds;
			arrowPeakY -= 250 * deltaTimeSeconds;
		}
	}
	//score
	modelMatrixScore = glm::mat3(1, 0, 0, 0, 1, 0, scoreBarX, scorebarY, 1);
	if (score >= 0) {
		modelMatrixScore *= Transform2d::Translate(scoreBarX + (score / 2), 0);
		modelMatrixScore *= Transform2d::Scale(score, 1);
		RenderMesh2D(meshes["posScoreBar"], shaders["VertexColor"], modelMatrixScore);
	}
	else if (score < 0) {
		modelMatrixScore *= Transform2d::Translate(scoreBarX + (score / 2), 0);
		modelMatrixScore *= Transform2d::Scale(-score, 1);
		RenderMesh2D(meshes["negScoreBar"], shaders["VertexColor"], modelMatrixScore);
		
	}

	//humanBody
	modelMatrixHB = glm::mat3(1, 0, 0, 0, 1, 0, bowx - 25, bowy, 1);
	modelMatrixBow *= Transform2d::Translate(translateX, translateY);
	RenderMesh2D(meshes["humanBody"], shaders["VertexColor"], modelMatrixHB);
	//BOW
	modelMatrixBow = glm::mat3(1, 0, 0, 0, 1, 0, bowx, bowy, 1);
	modelMatrixBow *= Transform2d::Translate(translateX, translateY);
	modelMatrixBow *= Transform2d::Rotate(bow_pointer);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], modelMatrixBow);

	//perimetrul power bar-ului
	ModelMatrixPerimPB = glm::mat3(1, 0, 0, 0, 1, 0, bowx + arrowLength / 4, bowy - bow_radius - 10, 1);
	ModelMatrixPerimPB *= Transform2d::Translate(translateX, translateY);
	RenderMesh2D(meshes["perimPB"], shaders["VertexColor"], ModelMatrixPerimPB);

	//ARROW
	modelMatrixArrow = glm::mat3(1, 0, 0, 0, 1, 0, arrowThrounX, arrowThrounY, 1);
	modelMatrixArrow *= Transform2d::Translate(translateX, translateY);
	if (!arrowThrown) {
		modelMatrixArrow *= Transform2d::Rotate(arrow_pointer);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);
		//area power bar
		ModelMatrixPerimPB = glm::mat3(1, 0, 0, 0, 1, 0, bowx + arrowLength / 4, bowy - bow_radius - 10, 1);
		ModelMatrixPerimPB *= Transform2d::Translate(translateX, translateY);
		if (arrowSpeed > 0 && arrowSpeed < arrowMaxSpeed) {
			areaPBScale += arrowLength / (1.7f * arrowSpeed) * timeSpent;
		}
		ModelMatrixPerimPB *= Transform2d::Scale(areaPBScale, 1);
		RenderMesh2D(meshes["areaPB"], shaders["VertexColor"], ModelMatrixPerimPB);
	}
	if (arrowThrown) {
		arrowThrounX += arrowSpeed * cos(arrow_pointer);
		arrowThrounY += arrowSpeed * sin(arrow_pointer);

		arrowPeakX = arrowThrounX + cos(arrow_pointer) * (arrowLength+arrowSide);
		arrowPeakY = arrowThrounY + sin(arrow_pointer) * (arrowLength+arrowSide);

		modelMatrixArrow *= Transform2d::Translate(arrowSpeed * cos(arrow_pointer), arrowSpeed * sin(arrow_pointer));
		modelMatrixArrow *= Transform2d::Rotate(arrow_pointer);
		RenderMesh2D(meshes["arrow"], shaders["VertexColor"], modelMatrixArrow);

		//verifix daca sageata a iesit din ecran
		if (arrowThrounX > resx || arrowThrounX < 0
			|| arrowThrounY > resy || arrowThrounY < 0) {
			arrowThrown = false;
			arrowThrounX = bowx;
			arrowThrounY = bowy;
			arrowSpeed = 0;
			areaPBScale = 1;
			timeSpent = 0;
		}
	}

	//HEARTS
	for (int i = 0; i < lifes; i++) {
		modelMatrixHeart = glm::mat3(1, 0, 0, 0, 1, 0, resx - (40 * (i+1)), resy - 25, 1);
		RenderMesh2D(meshes["heart"], shaders["VertexColor"], modelMatrixHeart);
	}


	//BALLOONS
	for (int i = 0; i < ballonsNumber; i++) {
		if (balloonsY[i] < resy && balloonIsHit[i] == 0) {
			modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i], balloonsY[i], 1);
			modelMatrixBalloon *= Transform2d::Translate(0, 60 * deltaTimeSeconds);
			balloonsY[i] += 60 * deltaTimeSeconds * balloonsSpeed[i];
			if (ballonsColor[i] == 0) {
				RenderMesh2D(meshes["redBalloon"], shaders["VertexColor"], modelMatrixBalloon);
			}
			else if (ballonsColor[i] == 1) {
				RenderMesh2D(meshes["yellowBalloon"], shaders["VertexColor"], modelMatrixBalloon);
			}
		}
		else if (balloonIsHit[i] == 1) {
			if (ballonsColor[i] == 0) {
				modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i]-5, balloonsY[i]+20, 1);
				RenderMesh2D(meshes["leftPartBalloonRed"], shaders["VertexColor"], modelMatrixBalloon);

				modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i]+5, balloonsY[i]-20, 1);
				RenderMesh2D(meshes["rightPartBalloonRed"], shaders["VertexColor"], modelMatrixBalloon);
				balloonsY[i] -= 60 * deltaTimeSeconds * balloonsSpeed[i];
			}
			else if (ballonsColor[i] == 1) {
				modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i] - 5, balloonsY[i] + 20, 1);
				RenderMesh2D(meshes["leftPartBalloonYellow"], shaders["VertexColor"], modelMatrixBalloon);

				modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i] + 5, balloonsY[i] - 20, 1);
				RenderMesh2D(meshes["rightPartBalloonYellow"], shaders["VertexColor"], modelMatrixBalloon);
				balloonsY[i] -= 60 * deltaTimeSeconds * balloonsSpeed[i];
			}
			if (balloonsY[i] <= balloon_radiusY && balloonIsHit[i] == 1)
				balloonIsHit[i] = 0;
		}
		else if (balloonsY[i] > resy) {
			ballonsColor[i] = rand() % 2;
			modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, balloonsX[i], 0, 1);
			balloonsY[i] = 0;
			if (ballonsColor[i] == 0) {
				RenderMesh2D(meshes["redBalloon"], shaders["VertexColor"], modelMatrixBalloon);
			}
			else if (ballonsColor[i] == 1) {
				RenderMesh2D(meshes["yellowBalloon"], shaders["VertexColor"], modelMatrixBalloon);
			}
			balloonIsHit[i] = 0;
		}
		//coliziune ballon-arrowPeak
		if (balloonIsHit[i] == 0 && distance(glm::vec3(arrowPeakX, arrowPeakY, 0), glm::vec3(balloonsX[i], balloonsY[i], 0)) <= balloon_radiusY) {
			balloonIsHit[i] = 1;
			timeSpent = 0;
			if (ballonsColor[i] == 0) { //red
				score += 2;
			}
			else if (ballonsColor[i] == 1) {
				score -= 1;
			}
		}
	}

	//SHURIKENS
	shurikenRotation += 15 * deltaTimeSeconds;
	for (int i = 0; i < shurikenNumber; i++) {
		if (shurikensX[i] > 0) {
			modelMatrixShuriken = glm::mat3(1, 0, 0, 0, 1, 0, shurikensX[i], shurikensY[i], 1);
			modelMatrixShuriken *= Transform2d::Rotate(shurikenRotation);
			shurikensX[i] -= 40 * deltaTimeSeconds * shurikensSpeed[i];
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
		}
		else if (shurikensX[i] < 0) {
			modelMatrixShuriken = glm::mat3(1, 0, 0, 0, 1, 0, resx, shurikensY[i], 1);
			shurikensX[i] = resx;
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], modelMatrixShuriken);
		}
		//coliziune shuriken-bow
		if (bowx + bow_radius >= shurikensX[i] - radius_shuriken && //verificare in dreapta
			(bowy - bow_radius <= shurikensY[i] + radius_shuriken && bowy + bow_radius >= shurikensY[i] - radius_shuriken)) { //jos -> sus
			shurikensX[i] = resx;
			lifes -= 1;
			if (lifes == 0) {
				finished = true;
			}
		}
		
		//coliziune shuriken-arrowPeak
		if (arrowThrown && distance(glm::vec3(arrowPeakX, arrowPeakY, 0), glm::vec3(shurikensX[i], shurikensY[i], 0)) <= radius_shuriken) {
			shurikensX[i] = resx;
			score += 3;
			timeSpent = 0;
		}
	}

	//radomObj
	if (timeSpentForRndObj >= 5) {
		if (randomObject == 0) {
			modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, randomObjectX, randomObjectY, 1);
			RenderMesh2D(meshes["ball1"], shaders["VertexColor"], modelMatrixBalloon);
		}
		else if (randomObject == 1) {
			modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, randomObjectX, randomObjectY, 1);
			RenderMesh2D(meshes["ball2"], shaders["VertexColor"], modelMatrixBalloon);
		}
		else if (randomObject == 2) {
			modelMatrixBalloon = glm::mat3(1, 0, 0, 0, 1, 0, randomObjectX, randomObjectY, 1);
			RenderMesh2D(meshes["ball3"], shaders["VertexColor"], modelMatrixBalloon);
		}

		if (randomObjectX > resx || randomObjectY > resy || randomObjectY < 0) {
			randomObject = rand() % 3;
			randomObjectX = 0, randomObjectY = 0;
			timeSpentForRndObj = 0;
			randomObjWasHit = false;
		}
		else if ((randomObject < resx && randomObjectY < resy)) {
			if (!randomObjWasHit) {
				randomObjectX += randomObjSpeed * cos(randomObjAng);
				randomObjectY += randomObjSpeed * sin(randomObjAng);
			}
			else if (randomObjWasHit) {
				randomObjectX += (randomObjSpeed/2) * cos(randomObjAng);
				randomObjectY -= randomObjSpeed * sin(randomObjAng);
			}
		}
		//coliziune arrowPeak-randomObj
		if ((arrowThrown && !randomObjWasHit) && 
				distance(glm::vec3(arrowPeakX, arrowPeakY, 0), glm::vec3(randomObjectX, randomObjectY, 0)) <= randomObjRadius) {
			score += 10;
			randomObjWasHit = true;
		}
	}
}

void Tema1::FrameEnd()
{

}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (mouseButtonIsPressed && arrowSpeed < arrowMaxSpeed) {
		arrowSpeed += 30 * deltaTime;
		timeSpent += deltaTime;
	}
	timeSpentForRndObj += deltaTime;
	cout << timeSpentForRndObj << endl;
	if (timeSpentForRndObj >= 4) {
		randomObjSpeed = 100 * deltaTime;
		cout << randomObjSpeed << endl;
	}
}

void Tema1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_W) {
		wIsPressed = true;
	}
	if (key == GLFW_KEY_S) {
		sIsPressed = true;
	}

}

void Tema1::OnKeyRelease(int key, int mods)
{
	// add key release event
	if (key == GLFW_KEY_W) {
		wIsPressed = false;
	}
	if (key == GLFW_KEY_S) {
		sIsPressed = false;
	}
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
	bow_pointer = atan2(resy - mouseY - bowy, mouseX - bowx);
	if (!arrowThrown) {
		arrow_pointer = atan2(resy - mouseY - bowy, mouseX - bowx);
	}
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
	if (!mouseButtonIsPressed && IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT) && !arrowThrown) {
		mouseButtonIsPressed = true;
	}
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
	mouseButtonIsPressed = false;
	arrowThrown = true;
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}