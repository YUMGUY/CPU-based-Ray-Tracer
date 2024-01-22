#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <chrono>
#include <iostream>
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "BVH.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include "main.h"

#define WINDOW_HEIGHT 800
#define WINDOW_WIDTH 1200

float frameBuffer[WINDOW_HEIGHT][WINDOW_WIDTH][3];
GLFWwindow *window;

std::vector<Shape*> shapes;
std::vector<Light*> lights;

// variables set for camera
glm::vec3 eyeScene = glm::vec3(0.0f, 0.0f, 7.0f);
glm::vec3 lookAt = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upV = glm::vec3(0.0f, 1.0f, 0.0f);
float fovYScene = 45.0f;
float focalDistScene = 1.0f;

// bunny task
std::vector<Triangle*> triangleVector;

// flags for bunny task
int showBunny = 0;



void ClearFrameBuffer()
{
	memset(&frameBuffer, 0, WINDOW_HEIGHT * WINDOW_WIDTH * 3 * sizeof(float));
}

void Display()
{	
	glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_FLOAT, frameBuffer);
}

void CharacterCallback(GLFWwindow* lWindow, unsigned int key)
{
	std::cout << "Key " << (char)key << " is pressed." << std::endl;
	switch (key) {
	case '6':
		// for debugging purposes in task 3
		//std::cout << "posBuff size:" << posBuff.size() << ", norBuff size:" << norBuff.size() << std::endl;
		std::cout << "Triangle Vector Size: " << triangleVector.size() << std::endl;
		break;
	}
	return;
}
// Create a vector of triangles. Considers the texture coordinates if they are available.
void CreateTriangleVector(std::vector<glm::vec3>& posBuff, std::vector<glm::vec3>& normCoords, bool makeBunny)
{
	for (int i = 0; i < 5; i++)
	{
		Triangle myTriangle;

		myTriangle = Triangle(posBuff[i * 3 + 0], posBuff[i * 3 + 1], posBuff[i * 3 + 2],
			normCoords[i * 3 + 0], normCoords[i * 3 + 1], normCoords[i * 3 + 2]);
	//	triangleVector.push_back(myTriangle);
		if (makeBunny) {
			shapes.push_back(new Triangle(posBuff[i * 3 + 0], posBuff[i * 3 + 1], posBuff[i * 3 + 2],
				normCoords[i * 3 + 0], normCoords[i * 3 + 1], normCoords[i * 3 + 2]));
		}
		
	}
}

void LoadModel(char* name)
{

	std::string meshName(name);
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes1;
	std::vector<tinyobj::material_t> materials;
	std::string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes1, &materials, &errStr, meshName.c_str());
	if (!rc) {
		std::cerr << errStr << std::endl;
	}
	else {
		// Loop over shapes
		for (size_t s = 0; s < shapes1.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for (size_t f = 0; f < shapes1[s].mesh.num_face_vertices.size(); f++) {
				std::vector<glm::vec3> vertices_;
				std::vector<glm::vec3> normals_;
				size_t fv = shapes1[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for (size_t v = 0; v < fv; v++) {
					// access to vertex
					glm::vec3 ve(0.0f);
					glm::vec3 no(0.0f);
					tinyobj::index_t idx = shapes1[s].mesh.indices[index_offset + v];
					ve = glm::vec3(attrib.vertices[3 * idx.vertex_index + 0],
												attrib.vertices[3 * idx.vertex_index + 1],
												attrib.vertices[3 * idx.vertex_index + 2]);
					if (!attrib.normals.empty()) {
						no = (glm::vec3(attrib.normals[3 * idx.vertex_index + 0],
													attrib.normals[3 * idx.vertex_index + 1],
													attrib.normals[3 * idx.vertex_index + 2]));
					}
					vertices_.push_back(ve);
					normals_.push_back(no);

			
				}
				triangleVector.push_back(new Triangle(vertices_[0], vertices_[1], vertices_[2], normals_[0], normals_[1], normals_[2]));

				index_offset += fv;
				// per-face material (IGNORE)
				shapes1[s].mesh.material_ids[f];
			}
		}
	}
}

void Init()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Assignment5 - Timothy Pham", NULL, NULL);
	glfwMakeContextCurrent(window);
	// added flags
	glfwSetCharCallback(window, CharacterCallback);

	glewExperimental = GL_TRUE;
	glewInit();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	ClearFrameBuffer();

	// For the Bunny Task, accept input 

	// set up lights
	Light* light1 = new Light(glm::vec3(0.0f, 3.0f, -2.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	Light* light2 = new Light(glm::vec3(-2.0f, 1.0f, 4.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	lights.push_back(light1);
	lights.push_back(light2);
	
	std::string input = "";
	
	std::cout << "Type and Choose 'Bunny' or 'Spheres': ";
	std::cin >> input;
	if (input == "Spheres") {

		std::cout << "Picked to render spheres" << std::endl;
		// set up shapes
		glm::vec3 ks_s1 = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 km_s1 = glm::vec3(0.0f, 0.0f, 0.0f);
		Shape* sphere1 = new Sphere(glm::vec3(-1.0f, -0.7f, 3.0f), 0.3f, glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.2, 1.0, 0.2), ks_s1, km_s1, 100.0f);

		glm::vec3 kd_s2 = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 ks_s2 = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 km_s2 = glm::vec3(0.0f, 0.0f, 0.0f);
		Shape* sphere2 = new Sphere(glm::vec3(1.0f, -0.5f, 3.0f), 0.5f, glm::vec3(0.1f), kd_s2, ks_s2, km_s2, 10.0f);

		glm::vec3 centerPos_s3 = glm::vec3(-1.0f, 0.0f, 0.0f);
		glm::vec3 ka_s3 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 kd_s3 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 ks_s3 = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 km_s3 = glm::vec3(1.0f, 1.0f, 1.0f);
		Shape* sphere3 = new Sphere(centerPos_s3, 1.0f, ka_s3, kd_s3, ks_s3, km_s3, 0.0f);

		glm::vec3 centerPos_s4 = glm::vec3(1.0f, 0.0f, -1.0f);
		glm::vec3 ka_s4 = glm::vec3(0.0f);
		glm::vec3 kd_s4 = glm::vec3(0.0f);
		glm::vec3 ks_s4 = glm::vec3(0.0f);
		glm::vec3 km_s4 = glm::vec3(0.8f);
		Shape* sphere4 = new Sphere(centerPos_s4, 1.0f, ka_s4, kd_s4, ks_s4, km_s4, 0.0f);

		shapes.push_back(sphere1);
		shapes.push_back(sphere2);
		shapes.push_back(sphere3);
		shapes.push_back(sphere4);

		// set planes
		glm::vec3 centerP1 = glm::vec3(0.0f, -1.0f, 0.0f);
		glm::vec3 normalP1 = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 ka_p1 = glm::vec3(0.1f);
		glm::vec3 kd_p1 = glm::vec3(1.0f);
		glm::vec3 ks_p1 = glm::vec3(0.0f);
		glm::vec3 km_p1 = glm::vec3(0.0f);
		Shape* plane1 = new Plane(centerP1, normalP1, ka_p1, kd_p1, ks_p1, km_p1, 0.0f);

		glm::vec3 centerP2 = glm::vec3(0.0f, 0.0f, -3.0f);
		glm::vec3 normalP2 = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 ka_p2 = glm::vec3(0.1f);
		glm::vec3 kd_p2 = glm::vec3(1.0f);
		glm::vec3 ks_p2 = glm::vec3(0.0f);
		glm::vec3 km_p2 = glm::vec3(0.0f);
		Shape* plane2 = new Plane(centerP2, normalP2, ka_p2, kd_p2, ks_p2, km_p2, 0.0f);

		shapes.push_back(plane1);
		shapes.push_back(plane2);
		showBunny = 0;

	}

	else {
		std::cout << "shapes vector filled with triangles" << std::endl;
		LoadModel("../obj/bunny.obj");
		
		// starts at 0
		BVH* head = new BVH(triangleVector, 0);
		shapes.push_back(head);
		showBunny = 1;
		// make head node, null left and right
		
		
	}



	Scene scene = Scene(shapes, lights, showBunny);
	Camera camera = Camera(eyeScene, lookAt, upV, fovYScene, focalDistScene, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	auto start = std::chrono::high_resolution_clock::now();
	camera.TakePicture(&scene, eyeScene, upV);                              // main loop of ray tracing
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << "Time passed (ms): " << duration.count() << std::endl;
	
	float *renderedImage = camera.GetRenderedImage();
	memcpy(frameBuffer, renderedImage, sizeof(float) * WINDOW_HEIGHT * WINDOW_WIDTH * 3);
}


int main()
{	
	Init();
	while ( glfwWindowShouldClose(window) == 0) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		Display();
		glFlush();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}