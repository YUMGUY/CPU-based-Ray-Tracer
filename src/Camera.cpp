#include "Camera.h"
#include <iostream>
Camera::Camera()
{
	this->eye = glm::vec3(0, 0, 0);
	this->lookAt = glm::vec3(0, 0, 0);
	this->up = glm::vec3(0, 0, 0);
	this->fovY = 0;
	this->focalDist = 0;

	this->widthRes = 1;
	this->heightRes = 1;
	renderedImage = new float[widthRes * heightRes * 3];

}

Camera::~Camera()
{
	delete renderedImage;
}

Camera::Camera(glm::vec3 eye_, glm::vec3 lookAt_, glm::vec3 up_, float fov_Y, float focalDist_, int widthR, int heightR)
{
	this->eye = eye_;
	this->lookAt = lookAt_;
	this->up = up_;
	this->fovY = fov_Y;
	this->focalDist = focalDist_;

	this->widthRes = widthR;
	this->heightRes = heightR;
	renderedImage = new float[widthRes * heightRes * 3];
}


// helper functions
glm::vec3 CalcRayColor(glm::vec3 origin, glm::vec3 ray, float t0, float t1, Scene* scene, int depthCounter) {
	glm::vec3 rayColor = { 0,0,0 };
	glm::vec3 Lvalue = glm::vec3(0.0f);
	glm::vec3 normalShape = glm::vec3(0.0f); // rn normal of sphere

	float epsilonValue = 0.001f;
	// call Hit, hit calls Intersect() from each shape in the shape vector from scene
	Shape recordedShape;
	if (scene->Hit(origin, ray, t0, t1, &recordedShape, normalShape) == true) {

		//// shading
		// ambient color, I ambient
		glm::vec3 color_ = recordedShape.ka; 
		// t value set in Hit()
		glm::vec3 intersectionPoint = origin + (recordedShape.t_value * ray);
		// base case, hard limit set to depth of 4
		if (depthCounter > 4) {
			return color_;
		}
	
		for (int i = 0; i < scene->lights.size(); ++i) {
		//    // left side of equation
		//	// L = lights.position - intersection point
			glm::vec3 calcedL_i = scene->lights.at(i)->lightPos - intersectionPoint;
			Lvalue = glm::normalize(calcedL_i);
			float light_t_Value = glm::dot(calcedL_i, normalShape);
			Shape shadowRecord;
			glm::vec3 shadowNormal;
			// gives shadows
			if (scene->Hit(intersectionPoint, Lvalue, epsilonValue, light_t_Value, &shadowRecord, shadowNormal) == false) {
				// left side of equation
				glm::vec3 diffuseV = recordedShape.kd * glm::max(0.0f, glm::dot(Lvalue, normalShape));
				glm::vec3 Rvalue = 2.0f * ((glm::dot(Lvalue, normalShape)) * normalShape) - Lvalue;
				// right side of equation
				Rvalue = glm::normalize(Rvalue);
				glm::vec3 Evalue = origin - intersectionPoint;
				Evalue = glm::normalize(Evalue);
				glm::vec3 specularV = recordedShape.ks * pow(glm::max(0.0f, glm::dot(Rvalue, Evalue)), recordedShape.n);
				glm::vec3 finalSum = diffuseV + specularV;

				// this is I direct, for reflection do I direct + km * I reflected
				glm::vec3 directV = scene->lights.at(i)->color * finalSum;
				color_ = color_ + directV;
			}
		}

		++depthCounter;
		glm::vec3 reflectRay = glm::reflect(ray, normalShape);
		color_ = color_ + (recordedShape.km * CalcRayColor(intersectionPoint, reflectRay, epsilonValue, t1, scene, depthCounter));
		return color_;
	}
	// background color
	else {
		return glm::vec3(0.0f, 0.0f, 0.0f);
	}

}

void Camera::TakePicture(Scene *scene, glm::vec3 eye_, glm::vec3 upVector)
{
	// construct camera frame
	glm::vec3 aVector = eye_ - lookAt;
	glm::vec3 bVector = upVector;

	glm::vec3 wVector = aVector / glm::length(aVector);
	glm::vec3 uVector = glm::cross(bVector, wVector) / glm::length(glm::cross(bVector, wVector));
	glm::vec3 v_Vector = glm::cross(wVector, uVector);

	float lyValue;
	float lxValue;
	float pwValue;

	// loop over all pixels
	memset(renderedImage, 0, sizeof(float) * widthRes * heightRes * 3);

	for (int y = 0; y < heightRes; ++y) {
		for (int x = 0; x < widthRes; ++x) {
			// create primary ray
			lyValue = 2.0f * focalDist * tan(fovY / 2.0f);
			lxValue = lyValue * ((float)widthRes/(heightRes));
			pwValue = lyValue / heightRes; // pixel width

			glm::vec3 originPoint = (glm::normalize(lookAt - eye) * focalDist) + eye - ((lxValue / 2.0f) * uVector) - ((lyValue / 2.0f) * v_Vector);
			glm::vec3 pixelCoord = originPoint + pwValue * (x + .5f) * uVector + pwValue * (y + .5f) * v_Vector;

			glm::vec3 primaryRay = glm::normalize(pixelCoord - eye);
			// create ray color(recursive)
			glm::vec3 computedRayColor = CalcRayColor(eye, primaryRay, 0.0f, INFINITY, scene, 0); // counter starts at 0

			// set color of pixel to ray color
			renderedImage[y * widthRes * 3 + x * 3 + 0] = computedRayColor.r;
			renderedImage[y * widthRes * 3 + x * 3 + 1] = computedRayColor.g;
			renderedImage[y * widthRes * 3 + x * 3 + 2] = computedRayColor.b;
		}
	}

	
}
