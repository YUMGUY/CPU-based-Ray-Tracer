#pragma once
#include "Shape.h"


class Sphere :
	public Shape
{
public:
	Sphere();
	Sphere(glm::vec3 centerPos_, float radius_, glm::vec3 ka_, glm::vec3 kd_, glm::vec3 ks_, glm::vec3 km_, float n_);
	float Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_);
	glm::vec3 CalcNormalVector(glm::vec3 intersectionPoint);
	~Sphere();

	
	float radius;
	

private:

};
