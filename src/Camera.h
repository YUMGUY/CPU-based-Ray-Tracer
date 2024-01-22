#pragma once
#include <vector>
#include "Scene.h"



class Camera
{
public:
	Camera();
	
	~Camera();
	Camera(glm::vec3 eye_, glm::vec3 lookAt_, glm::vec3 up_, float fov_Y, float focalDist_, int widthR, int heightR);

	void TakePicture(Scene *scene, glm::vec3 eye_, glm::vec3 upVector);
	float* GetRenderedImage() { return renderedImage; };


	// camera properties
	glm::vec3 eye;
	glm::vec3 lookAt;
	glm::vec3 up; 
	float fovY;
	float focalDist;

private:

	int widthRes;
	int heightRes;

	float *renderedImage;


};
