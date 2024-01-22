#pragma once
#include "Shape.h"

class Triangle :
	public Shape
{
public:
	Triangle();
	Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& n0, glm::vec3& n1, glm::vec3& n2);
	float Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_);
	glm::vec3 CalcNormalVector(glm::vec3 intersectionPoint);
	~Triangle();

	
	glm::vec3 normalT;
	glm::vec3 v[3]; // carries values of posBuff?
	glm::vec3 n_[3]; // carries the values of norBuff?

	float betaValue;
	float gammaValue;
	// need color vector?


private:

};
