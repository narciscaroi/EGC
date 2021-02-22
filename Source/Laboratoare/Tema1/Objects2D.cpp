#include "Objects2D.h"

#include <Core/Engine.h>
const float PI = 3.1415f;

Mesh* Objects2D::CreateBow(std::string name, float x, float y, float radius) {

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float currAngle;

	Mesh* circle = new Mesh(name);

	for (currAngle = -PI/2; currAngle < PI/2; currAngle += 0.0001f) {
		vertices.push_back(VertexFormat(x + glm::vec3(radius * cos(currAngle), y +  radius * sin(currAngle), 0), glm::vec3(0.2f, 0.9f, 0.8f)));
	}

	for (unsigned short i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	circle->SetDrawMode(GL_LINE_LOOP);

	circle->InitFromData(vertices, indices);

	return circle;
}

Mesh* Objects2D::CreatePerimeterPowerBar(std::string name, float length, float side) {
	glm::vec3 color = glm::vec3(0.05f, 0.98f, 0.02f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-length/2,-side/2,0), color),
		VertexFormat(glm::vec3(length/2, -side/2, 0), color),
		VertexFormat(glm::vec3(length/2, side/2, 0), color),
		VertexFormat(glm::vec3(-length/2, side/2, 0), color)
	};

	std::vector<unsigned short> indices;
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* perimPB = new Mesh(name);
	perimPB->SetDrawMode(GL_LINE_LOOP);

	perimPB->InitFromData(vertices, indices);

	return perimPB;
}

Mesh* Objects2D::CreateAreaPowerBar(std::string name, float length, float side, int clr) {
	glm::vec3 color;
	if (clr == 0) {
		color = glm::vec3(0.05f, 0.98f, 0.02f);
	}
	else if (clr == 1) {
		color = glm::vec3(1, 0, 0.4f);
	}
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-length/2,-side/2,0), color),
		VertexFormat(glm::vec3(length/2, -side/2, 0), color),
		VertexFormat(glm::vec3(length/2, side/2, 0), color),
		VertexFormat(glm::vec3(-length/2 , side/2, 0), color)
	};

	std::vector<unsigned short> indices;
	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* areaPB = new Mesh(name);
	areaPB->SetDrawMode(GL_QUADS);

	areaPB->InitFromData(vertices, indices);

	return areaPB;
}

Mesh* Objects2D::CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, float side) {
	glm::vec3 corner = leftBottomCorner;
	glm::vec3 color = glm::vec3(0.2f, 0.9f, 0.8f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, -side / 2, 0), color),//0
		VertexFormat(corner + glm::vec3(length, -side/2, 0), color), //1
		VertexFormat(corner + glm::vec3(length, -side, 0), color),//2
		VertexFormat(corner + glm::vec3(length + side, 0, 0), color), // 3
		VertexFormat(corner + glm::vec3(length, side, 0), color), // 4
		VertexFormat(corner + glm::vec3(length, side/2, 0), color), //5
		VertexFormat(corner + glm::vec3(0, side/2, 0), color) //6
	};

	std::vector<unsigned short> indices = {
		0, 1, 5,
		0, 5, 6,
		2, 3, 4
	};

	Mesh* arrow = new Mesh(name);

	arrow->SetDrawMode(GL_TRIANGLES);

	arrow->InitFromData(vertices, indices);

	return arrow;
}


Mesh* Objects2D::CreateBalloon(std::string name, float radiusX, float radiusY, float x, float y, int clr) {
	glm::vec3 color;
	if (clr == 0) {
		color = glm::vec3(1, 0, 0.4f);
	}
	else if (clr == 1) {
		color = glm::vec3(1, 1, 0);
	}
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	//desenarea formei ovale
	for (float currAngle = -PI/2; currAngle < 3*PI/2; currAngle+=0.0005) {
		vertices.push_back(VertexFormat(glm::vec3(x, y, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(x + radiusX * cos(currAngle), y + radiusY * sin(currAngle), 0), color));
	}

	//creez nodul de la ballon cu un triunghi
	int base_triangle = 10;
	for (float i = 0; i < base_triangle * 2; i += 0.01) {
		vertices.push_back(VertexFormat(glm::vec3(x, -radiusY, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(x -base_triangle + i, -radiusY-10, 0), color));
	}


	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			vertices.push_back(VertexFormat(glm::vec3(x - base_triangle, -radiusY - (20 * (i+1)), 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x + base_triangle, -radiusY - (20 * (i+2)), 0), color));
		}
		else {
			vertices.push_back(VertexFormat(glm::vec3(x + base_triangle, -radiusY - (20 * (i+1)), 0), color));
			vertices.push_back(VertexFormat(glm::vec3(x - base_triangle, -radiusY - (20 * (i + 2)), 0), color));
		}
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}


	Mesh* balloon = new Mesh(name);

	balloon->SetDrawMode(GL_LINE_STRIP);

	balloon->InitFromData(vertices, indices);

	return balloon;
}


Mesh* Objects2D::CreateHeart(std::string name) {
	glm::vec3 color = glm::vec3(0.98f, 0.02f, 0.16f);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float currAngle;

	for (currAngle = 0; currAngle <= 2 * PI; currAngle += 0.001f) {
		vertices.push_back(VertexFormat(glm::vec3(0, 0, 0), color));
		vertices.push_back(VertexFormat(glm::vec3
											(16.0 * sin(currAngle) * sin(currAngle) * sin(currAngle),
											13.0 * cos(currAngle) - 5.0 * cos(2.0 * currAngle) - 2.0 * cos(3.0 * currAngle) - cos(4.0 * currAngle),
											0), color));
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* heart = new Mesh(name);

	heart->SetDrawMode(GL_LINE_STRIP);

	heart->InitFromData(vertices, indices);

	return heart;
}

Mesh* Objects2D::CreateShuriken(std::string name, float x, float y) {
	glm::vec3 color = glm::vec3(0.2f, 0.9f, 0.8f);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0, 0), color),//0
		VertexFormat(glm::vec3(0, x, 0), color),//1
		VertexFormat(glm::vec3(y, x, 0), color),//2
		VertexFormat(glm::vec3(0, 0, 0), color),//3
		VertexFormat(glm::vec3(-x,0, 0), color),//4
		VertexFormat(glm::vec3(-x, y, 0), color),//5
		VertexFormat(glm::vec3(0, 0, 0), color),//6
		VertexFormat(glm::vec3(0, -x, 0), color),//7
		VertexFormat(glm::vec3(-y, -x, 0), color),//8
		VertexFormat(glm::vec3(0, 0, 0), color),//9
		VertexFormat(glm::vec3(x, 0, 0), color),//10
		VertexFormat(glm::vec3(x, -y, 0), color),//11
	};
	
	std::vector<unsigned short> indices = {
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11
	};

	Mesh* shuriken = new Mesh(name);

	shuriken->SetDrawMode(GL_TRIANGLES);

	shuriken->InitFromData(vertices, indices);

	return shuriken;
}

Mesh* Objects2D::CreateLeftPartBallon(std::string name, float radiusX, float radiusY, float x, float y, int clr) {
	glm::vec3 color;
	if (clr == 0) {
		color = glm::vec3(1, 0, 0.4f);
	}
	else if (clr == 1) {
		color = glm::vec3(1, 1, 0);
	}
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	//desenarea formei ovale
	for (float currAngle = 0; currAngle < PI; currAngle += 0.0005) {
		vertices.push_back(VertexFormat(x + glm::vec3(x, y, 0), color));
		vertices.push_back(VertexFormat(x + glm::vec3(radiusX * cos(currAngle), y + radiusY * sin(currAngle), 0), color));
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* leftBalloonPart = new Mesh(name);

	leftBalloonPart->SetDrawMode(GL_LINE_STRIP);

	leftBalloonPart->InitFromData(vertices, indices);

	return  leftBalloonPart;

}
Mesh* Objects2D::CreateRightPartBallon(std::string name, float radiusX, float radiusY, float x, float y, int clr) {
	glm::vec3 color;
	if (clr == 0) {
		color = glm::vec3(1, 0, 0.4f);
	}
	else if (clr == 1) {
		color = glm::vec3(1, 1, 0);
	}
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	//desenarea formei ovale
	for (float currAngle = 0; currAngle < PI; currAngle += 0.0005) {
		vertices.push_back(VertexFormat(x + glm::vec3(x, y, 0), color));
		vertices.push_back(VertexFormat(x + glm::vec3(radiusX * cos(currAngle), y - radiusY * sin(currAngle), 0), color));
	}

	//creez nodul de la ballon cu un triunghi
	int base_triangle = 10;
	for (float i = 0; i < base_triangle * 2; i += 0.01) {
		vertices.push_back(VertexFormat(x + glm::vec3(x, -radiusY, 0), color));
		vertices.push_back(VertexFormat(x + glm::vec3(-base_triangle + i, -radiusY - 10, 0), color));
	}


	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			vertices.push_back(VertexFormat(x + glm::vec3(-base_triangle, -radiusY - (20 * (i + 1)), 0), color));
			vertices.push_back(VertexFormat(x + glm::vec3(base_triangle, -radiusY - (20 * (i + 2)), 0), color));
		}
		else {
			vertices.push_back(VertexFormat(x + glm::vec3(base_triangle, -radiusY - (20 * (i + 1)), 0), color));
			vertices.push_back(VertexFormat(x + glm::vec3(-base_triangle, -radiusY - (20 * (i + 2)), 0), color));
		}
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}


	Mesh* rightBalloonPart = new Mesh(name);

	rightBalloonPart->SetDrawMode(GL_LINE_STRIP);

	rightBalloonPart->InitFromData(vertices, indices);

	return rightBalloonPart;
}

Mesh* Objects2D::CreateHumanBody(std::string name, float centerX, float centerY)
{
	glm::vec3 color = glm::vec3(0.7f, 0.6f, 0.1f);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;
	float headRadius = 15, bodyLength = 100, armLength = 25, footLength = 25, feetSide = 14;
	int centerTo1Arm = 20, firstArmTo2Arm = 20, sndArmToNeck = 10, centerToFoot = bodyLength / 2 + 15;

	//centru si prima mana
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + centerTo1Arm, 0), color));
	//prima mana
	vertices.push_back(VertexFormat(glm::vec3(centerX + armLength, centerY, 0), color));
	//prima mana si a doua mana
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + centerTo1Arm, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + centerTo1Arm + firstArmTo2Arm, 0), color));
	//a doua mana
	vertices.push_back(VertexFormat(glm::vec3(centerX + armLength, centerY, 0), color));
	//a doua mana si "terminarea gatului"
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + centerTo1Arm + firstArmTo2Arm, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + bodyLength / 2, 0), color));

	//crearea capului
	float centerYCircle = centerY + bodyLength / 2 + headRadius;
	for (float currAng = -PI / 2; currAng < 3 * PI / 2; currAng += 0.001f) {
		vertices.push_back(VertexFormat(glm::vec3(centerX, centerYCircle, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(centerX + headRadius * cos(currAng), centerYCircle + headRadius * sin(currAng), 0), color));
	}

	//adaug punctul de la gat
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY + bodyLength / 2, 0), color));
	//unesc cu punctul de unde incep picioarele
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY - bodyLength / 2, 0), color));
	//adaug piciorul din stanga
	vertices.push_back(VertexFormat(glm::vec3(centerX - feetSide/2, centerY - bodyLength / 2 - footLength, 0), color));
	//punctul de unde incep picioarele si piciorul din dreapta
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY - bodyLength / 2, 0), color));
	vertices.push_back(VertexFormat(glm::vec3(centerX + feetSide / 2, centerY - bodyLength / 2 - footLength, 0), color));

	//punctul de la inceputul picioarelor
	vertices.push_back(VertexFormat(glm::vec3(centerX, centerY - bodyLength / 2, 0), color));

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}

	Mesh* humanBody = new Mesh(name);

	humanBody->SetDrawMode(GL_LINE_LOOP);

	humanBody->InitFromData(vertices, indices);

	return humanBody;
}

Mesh* Objects2D::CreateBall1(std::string name, float centerX, float centerY, float radius) {
	glm::vec3 color = glm::vec3(1, 0, 0);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	for (float currAngle = 0; currAngle < 2 * PI; currAngle += 0.1f) {
		vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(centerX + radius * cos(currAngle), centerY + radius * sin(currAngle), 0), color));
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}
	Mesh* ball = new Mesh(name);
	ball->SetDrawMode(GL_LINE_STRIP);
	ball->InitFromData(vertices, indices);
	return ball;

}

Mesh* Objects2D::CreateBall2(std::string name, float centerX, float centerY, float radius) {
	glm::vec3 color = glm::vec3(0, 1, 0);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	for (float currAngle = 0; currAngle < 2 * PI; currAngle += 0.1f) {
		vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(centerX + radius * cos(currAngle), centerY + radius * sin(currAngle), 0), color));
		//vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}
	Mesh* ball = new Mesh(name);
	ball->SetDrawMode(GL_TRIANGLES);
	ball->InitFromData(vertices, indices);
	return ball;

}

Mesh* Objects2D::CreateBall3(std::string name, float centerX, float centerY, float radius) {
	glm::vec3 color = glm::vec3(1, 0, 1);
	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	float rad2deg = PI / 180;

	for (float currAngle = 0; currAngle < 360; currAngle += 1) {
		vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
		vertices.push_back(VertexFormat(glm::vec3(centerX + radius * cos(currAngle * rad2deg), 
												 centerY + radius * sin(currAngle * rad2deg), 0), color));
		//vertices.push_back(VertexFormat(glm::vec3(centerX, centerY, 0), color));
	}

	for (int i = 0; i < vertices.size(); i++) {
		indices.push_back(i);
	}
	Mesh* ball = new Mesh(name);
	ball->SetDrawMode(GL_TRIANGLES);
	ball->InitFromData(vertices, indices);
	return ball;

}