#pragma once
#include <glm/glm.hpp>
class Light
{
public:
	Light(glm::vec3 lightPos_, glm::vec3 color_);
	~Light();

	glm::vec3 lightPos;
	glm::vec3 color;


private:

};
