#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Objects2D
{
	Mesh* CreateBow(std::string name, float x, float y, float length);
	Mesh* CreatePerimeterPowerBar(std::string name, float length, float side);
	Mesh* CreateAreaPowerBar(std::string name, float length, float side, int clr);
	Mesh* CreateArrow(std::string name, glm::vec3 leftBottomCorner, float length, float side);
	Mesh* CreateBalloon(std::string name, float radiusX, float radiusY, float x, float y, int clr);
	Mesh* CreateShuriken(std::string name);
	Mesh* CreateHeart(std::string name);
	Mesh* CreateShuriken(std::string name, float c1, float c2);

	//baloane separate
	Mesh* CreateLeftPartBallon(std::string name, float radiusX, float radiusY, float x, float y, int clr);
	Mesh* CreateRightPartBallon(std::string name, float radiusX, float radiusY, float x, float y, int clr);

	Mesh* CreateHumanBody(std::string name, float centerX, float centerY);
	Mesh* CreateBall1(std::string name, float centerX, float centerY, float radius);
	Mesh* CreateBall2(std::string name, float centerX, float centerY, float radius);
	Mesh* CreateBall3(std::string name, float centerX, float centerY, float radius);
}