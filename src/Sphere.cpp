#include "Sphere.h"
#include <iostream>
Sphere::Sphere()
{
	this->center = glm::vec3(0, 0, 0);
	this->radius = 0.0f;
	this->ka = glm::vec3(0, 0, 0);
	this->kd = glm::vec3(0, 0, 0);
	this->ks = glm::vec3(0, 0, 0);
	this->km = glm::vec3(0, 0, 0);
	this->n = 0.0f;

	this->t_value = 0.0f;
	this->normalValue = glm::vec3(0.0f);
}

Sphere::Sphere(glm::vec3 centerPos_, float radius_, glm::vec3 ka_, glm::vec3 kd_, glm::vec3 ks_, glm::vec3 km_, float n_)
{
	this->center = centerPos_;
	this->radius = radius_;
	this->ka = ka_;
	this->kd = kd_;
	this->ks = ks_;
	this->km = km_;
	this->n = n_;

	this->t_value = 0.0f;
	this->normalValue = glm::vec3(0.0f); // this keeps on overriding values
}

float Sphere::Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_) {
	// origin might not be calculated correctly
	// slide 14,
	float tValue1 = 0.0f;
	float tValue2 = 0.0f;

	float aValue = glm::dot(ray, ray);
	float bValue = glm::dot(2.0f * (origin - center), ray);
	float cValue = glm::dot((origin - center), origin - center) - pow(radius, 2);
	// quadratic formula
	float determinantValue = pow(bValue, 2) - (4.0f * aValue * cValue);
	if (determinantValue < t0) { // no intersection
		return INFINITY;
	}
	tValue1 = ( (-1.0f * bValue) + sqrt(determinantValue) ) / (2.0f * aValue);
	tValue2 = ( (-1.0f * bValue) - sqrt(determinantValue) ) / (2.0f * aValue);

	float min_tValue = std::min(tValue1, tValue2);
	
	// calced differntly for each shape, passed as reference
	/*glm::vec3 intersect = (origin + (min_tValue * ray));
	glm::vec3 normalValueCalc = glm::normalize(intersect - center);*/
	//normal_ = normalValueCalc;
	// check between bounds
	if (t0 < min_tValue && min_tValue < t1) {
		return min_tValue;
	}
	else {
		return INFINITY;
	}

}
// Calculation of the Normal for a Sphere
glm::vec3 Sphere::CalcNormalVector(glm::vec3 intersectionPoint) {
	glm::vec3 normalValueCalc = glm::normalize(intersectionPoint - center);
	return normalValueCalc;
}

Sphere::~Sphere()
{
}

