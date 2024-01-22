#pragma once
#include "Shape.h"
#include <vector>
#include "Triangle.h"
class BVH :
	public Shape
{
public:
	BVH();
	BVH(std::vector<Triangle*>  tv, int depth);
	float Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_);
	glm::vec3 CalcNormalVector(glm::vec3 intersectionPoint);
	~BVH();

	std::vector<Triangle*> trianglesV;

private:
	
	BVH* leftB = nullptr;
	BVH* rightB = nullptr;

	float maxXValue = FLT_MIN;
	float maxYValue = FLT_MIN;
	float maxZValue = FLT_MIN;
	float minXValue = FLT_MAX;
	float minYValue = FLT_MAX;
	float minZValue = FLT_MAX;

	

};
