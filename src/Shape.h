#pragma once
#include <glm/glm.hpp>
class Shape
{
public:
	Shape(void);
	Shape(glm::vec3 center_, glm::vec3 ka_, glm::vec3 kd_, glm::vec3 ks_, glm::vec3 km_, float n_);
	virtual float Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_);
	virtual glm::vec3 CalcNormalVector(glm::vec3 intersectionPoint);
	~Shape(void);

	// shared variables for Plane and Sphere
	glm::vec3 center;

	glm::vec3 ka;
	glm::vec3 kd;
	glm::vec3 ks;
	glm::vec3 km;
	float n;

	// record t -value based on hit
	float t_value;

	// another shared potential variable
	glm::vec3 normalValue;

private:
	
};
