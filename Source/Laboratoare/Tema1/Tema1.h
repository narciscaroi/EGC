#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
public:
	Tema1();
	~Tema1();

	void Init() override;

private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;

	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;

protected:
	glm::mat3 modelMatrixBow, modelMatrixArrow, ModelMatrixPerimPB, modelMatrixHB;
	glm::mat3 modelMatrixBalloon, modelMatrixHeart, modelMatrixShuriken;
	glm::mat3 modelMatrixScore;
	float translateX, translateY;
	float scaleX, scaleY;
	float angularStep;
	float rotation;
	float resx, resy;
	bool mouseButtonIsPressed;
	bool finished;
	float score;

	//bow
	float bow_pointer;
	float bowx, bowy, bow_radius;
	//powerbar
	float areaPBScale;
	float scoreBarX, scorebarY;
	//arrow
	float arrow_pointer;
	float arrowThrounX, arrowThrounY, arrowPeakX, arrowPeakY;
	float arrowSpeed;
	bool arrowThrown;
	float arrowLength, arrowSide;
	float const arrowMaxSpeed = 150;
	//heart
	float hearts[3];
	float lifes;
	//ballons
	float balloon_radiusX, balloon_radiusY;
	float const balloonBaseH = 10;
	float ballonsNumber;
	float balloonsX[5], balloonsY[5], ballonsColor[5], balloonsSpeed[5], balloonIsHit[5];
	//shurikens
	float radius_shuriken, cy;
	float const shurikenNumber = 5;
	float shurikensX[5], shurikensY[5], shurikensSpeed[5];
	float shurikenRotation;

	//randomObj
	int randomObject;
	float randomObjectX, randomObjectY, randomObjRadius;
	float timeSpentForRndObj, randomObjAng, randomObjSpeed;
	bool randomObjWasHit;
};