#pragma once
#include <vector>
#include "Shape.h"
#include "Light.h"


class Scene
{
public:
	Scene(std::vector<Shape*> shapes_, std::vector<Light*> lights_, int flagInput_);
	bool Hit(glm::vec3 origin, glm::vec3 ray, float t0, float t1, Shape* rec, glm::vec3& normal_);
	~Scene();
	std::vector<Shape*> shapes;
	std::vector<Light*> lights;
	int flag;

private:
	

};
