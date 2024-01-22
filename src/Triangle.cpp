#include <iostream>
#include "Triangle.h"

Triangle::Triangle()
{
	this->normalT = glm::vec3(0.0f);

	this->normalValue = glm::vec3(0.0f);

	v[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	v[2] = glm::vec3(0.0f, 0.0f, 0.0f);

	n_[0] = glm::vec3(0.0f, 0.0f, 0.0f);
	n_[1] = glm::vec3(0.0f, 0.0f, 0.0f);
	n_[2] = glm::vec3(0.0f, 0.0f, 0.0f);
	this->t_value = 0.0f;

}

Triangle::Triangle(glm::vec3& v0, glm::vec3& v1, glm::vec3& v2, glm::vec3& n0, glm::vec3& n1, glm::vec3& n2)
{
	float centerX = (v[0].x + v[1].x + v[2].x )/ 3.0f;
	float centerY = (v[0].y + v[1].y + v[2].y) / 3.0f;
	float centerZ = (v[0].z + v[1].z + v[2].z) / 3.0f;
	center = glm::vec3(centerX, centerY, centerZ);

	this->normalT = glm::vec3(0.0f);


	this->ka = glm::vec3(0.1f);
	this->kd = glm::vec3(0.0f, 0.0f, 1.0f);
	this->ks = glm::vec3(1.0f, 1.0f, 0.5f);
	this->km = glm::vec3(0.0f);
	this->n = 100.0f;


	this->t_value = 0.0f;
	

	v[0] = v0;
	v[1] = v1;
	v[2] = v2;

	n_[0] = n0;
	n_[1] = n1;
	n_[2] = n2;

	this->betaValue = 0.0f;
	this->gammaValue = 0.0f;

}

float Triangle::Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_) {
	
	//// muller trombore
	
	float gammaValue = 0.0f;
	float betaValue = 0.0f;

	glm::vec3 Evector1 = v[1] - v[0];
	glm::vec3 Evector2 = v[2] - v[0];

	glm::vec3 Svector = origin - v[0]; // tvec
	glm::vec3 Svector1 = glm::cross(glm::normalize(ray), Evector2); // pvec	
	glm::vec3 Svector2 = glm::cross(Svector, Evector1);

	float dotSE = 1.0f / glm::dot(Svector1, Evector1);

	float u = glm::dot(Svector, Svector1) * dotSE;
	if (u < 0.0f || u > 1.0f) {
		return INFINITY;
	}

	glm::vec3 qVec = glm::cross(Svector, Evector1);

	float v = glm::dot(ray, qVec) * dotSE;
	if (v < 0.0f || u + v > 1.0f) {
		return INFINITY;
	}


	float S2dotE2 = glm::dot(Svector2, Evector2);
	float S1dotS = glm::dot(Svector1, Svector);
	float S2dotD = glm::dot(Svector2, ray);



	/*float t_Value_Calced = dotSE * S2dotE2;*/
	this->betaValue = dotSE * S1dotS;
	this->gammaValue = dotSE * S2dotD;
	//this->t_value = t_Value_Calced;
	float t_Value_Calced = glm::dot(Evector2, qVec) * dotSE;
	
	if (t0 < t_Value_Calced && t_Value_Calced < t1) {
		return t_Value_Calced;
	}
	else {
		return INFINITY;
	}
}

glm::vec3 Triangle::CalcNormalVector(glm::vec3 intersectionPoint) {

	float alpha = 1.0f - betaValue - gammaValue;

	// interpolate normal
	normalT = alpha * n_[0] + betaValue * n_[1] + gammaValue * n_[2];
 	// using barycentric coordinates (computed with Muller Trombore)
	return normalT;
}

Triangle::~Triangle()
{
}