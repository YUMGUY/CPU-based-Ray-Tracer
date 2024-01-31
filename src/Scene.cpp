#include "Scene.h"
#include <iostream>
Scene::Scene(std::vector<Shape*> shapes_, std::vector<Light*> lights_, int flagInput_): shapes(shapes_), lights(lights_), flag(flagInput_)
{

}

bool Scene::Hit(glm::vec3 origin, glm::vec3 ray, float t0, float t1, Shape* rec, glm::vec3& normal_) {
	float t_Value = 0.0f;
	// rec is assigned for each shape
	float min_t_Value = FLT_MAX;
	int shapeIndex = 0;
	bool hit = false;

	for (int i = 0; i < shapes.size(); ++i) {
		float t_Value = shapes.at(i)->Intersect(origin, ray, t0, t1, normal_); // t_Value is INFINITY if the intersection fails

		// the t_value that returns when not hit determines what t value is compared to in the conditional
		if (t_Value < t1 && t_Value < min_t_Value) { // hit
			
			hit = true;
			min_t_Value = t_Value;
			shapeIndex = i;
			(*rec) = (*shapes.at(i));
		}
	}

	rec->t_value = min_t_Value;
	
	// normal_ might have to be stored within the shapes normalValue
	if (flag == 0) { // not the bunny
		glm::vec3 intPoint = origin + (ray * min_t_Value);
		// normal calculated based on what shape it is
		normal_ = shapes.at(shapeIndex)->CalcNormalVector(intPoint);
	}


	return hit;
}

Scene::~Scene()
{
	// free memory in heap
	for (Shape* shapeInScene : shapes) {
		delete shapeInScene;
	}
	for (Light* lightInScene : lights) {
		delete lightInScene;
	}
}


