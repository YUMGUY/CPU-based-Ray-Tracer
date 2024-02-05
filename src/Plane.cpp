#include "Plane.h"


Plane::Plane()
{
	this->center = glm::vec3(0.0f);

	this->normalP = glm::vec3(0.0f);
	this->ka = glm::vec3(0.0f);
	this->kd = glm::vec3(0.0f);
	this->ks = glm::vec3(0.0f);
	this->km = glm::vec3(0.0f);
	this->n = 0.0f;

	this->t_value = 0.0f;
	this->normalValue = glm::vec3(0.0f);

}

Plane::Plane(glm::vec3 center_, glm::vec3 normalP_, glm::vec3 ka_, glm::vec3 kd_, glm::vec3 ks_, glm::vec3 km_, float n_)
{
	this->center = center_;

	this->normalP = normalP_; 
	this->ka = ka_; 
	this->kd = kd_; 
	this->ks = ks_; 
	this->km = km_; 
	this->n = n_;

	this->t_value = 0.0f;
	this->normalValue = normalP_;

}

float Plane::Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_) {

	float t_Value = 0.0f;

	float numerator = glm::dot(center - origin, normalP);
	float denom = glm::dot(ray, normalP);

	t_Value = numerator / denom;

	if (t0 <= t_Value && t_Value < t1) {
		return t_Value;
	}
	else {
		return INFINITY;
	}

}
// Calculation of the Normal Vector for a Plane
glm::vec3 Plane::CalcNormalVector(glm::vec3 intersectionPoint) {
	return normalP;
}

Plane::~Plane()
{
}

