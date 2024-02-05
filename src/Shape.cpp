#include "Shape.h"

Shape::Shape() :center({}), ka(glm::vec3(0.0f)), kd(glm::vec3(0.0f)), ks(glm::vec3(0.0f)), km(glm::vec3(0.0f)), n(0.0f), t_value(0.0f), normalValue({})
{
}

Shape::Shape(glm::vec3 center_ ,glm::vec3 ka_, glm::vec3 kd_, glm::vec3 ks_, glm::vec3 km_, float n_): center(center_), ka(ka_),
kd(kd_), ks(ks_), km(km_), n(n_),t_value(0.0f),normalValue({})
{
}

// return t value
float Shape::Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_) {
	return 1.0f;
}
// Overloaded by Shape child classes
glm::vec3 Shape::CalcNormalVector(glm::vec3 intersectionPoint) {
	return { 0.0f, 0.0f, 0.0f };
}

Shape::~Shape()
{
}

