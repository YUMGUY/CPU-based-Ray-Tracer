#include "BVH.h"
#include <iostream>
#include "Triangle.h"
#include <algorithm>
struct {
    bool operator()(Triangle* a, Triangle* b) const { return a->center.x < b->center.x; }
} XValue;

struct {
    bool operator()(Triangle* a, Triangle* b) const { return a->center.y < b->center.y; }
} YValue;

struct {
    bool operator()(Triangle* a, Triangle* b) const { return a->center.z < b->center.z; }
} ZValue;

BVH::BVH() {

}

BVH::BVH(std::vector<Triangle*> tv, int depth)
{
    this->ka = glm::vec3(0.1f);
    this->kd = glm::vec3(0.0f, 0.0f, 1.0f);
    this->ks = glm::vec3(1.0f, 1.0f, 0.5f);
    this->km = glm::vec3(0.0f);
    this->n = 100.0f;

    this->trianglesV = tv;

    // split into left and right
    int leftTriSize = this->trianglesV.size() / 2;
    int rightTriSize = this->trianglesV.size()/2;
    std::vector<Triangle*> leftTri;
    std::vector<Triangle*> rightTri;

    // define variables
    for each (Triangle* triangle in trianglesV) {
        
        if (triangle->v[1].x < minXValue) {
            minXValue = triangle->v[1].x;
        }
        if (triangle->v[2].x < minXValue) {
            minXValue = triangle->v[2].x;
        }

        if (triangle->v[0].y < minYValue) {
            minYValue = triangle->v[0].y;
        }
        if (triangle->v[1].y < minYValue) {
            minYValue = triangle->v[1].y;
        }
        if (triangle->v[2].y < minYValue) {
            minYValue = triangle->v[2].y;
        }

        if (triangle->v[0].z < minZValue) {
            minZValue = triangle->v[0].z;
        }
        if (triangle->v[1].z < minZValue) {
            minZValue = triangle->v[1].z;
        }
        if (triangle->v[2].z < minZValue) {
            minZValue = triangle->v[2].z;
        }

       
        if (triangle->v[0].x > maxXValue) {
            maxXValue = triangle->v[0].x;
        }
        if (triangle->v[1].x > maxXValue) {
            maxXValue = triangle->v[1].x;
        }
        if (triangle->v[2].x > maxXValue) {
            maxXValue = triangle->v[2].x;
        }

        if (triangle->v[0].y > maxYValue) {
            maxYValue = triangle->v[0].y;
        }
        if (triangle->v[1].y > maxYValue) {
            maxYValue = triangle->v[1].y;
        }
        if (triangle->v[2].y > maxYValue) {
            maxYValue = triangle->v[2].y;
        }

        if (triangle->v[0].z > maxZValue) {
            maxZValue = triangle->v[0].z;
        }
        if (triangle->v[1].z > maxZValue) {
            maxZValue = triangle->v[1].z;
        }
        if (triangle->v[2].z > maxZValue) {
            maxZValue = triangle->v[2].z;
        }
    }
   
    float avgXValue = (maxXValue - minXValue) / 2.0f;
    float avgYValue = (maxYValue - minYValue) / 2.0f;
    float avgZValue = (maxZValue - minZValue) / 2.0f;
    center = glm::vec3(avgXValue, avgYValue, avgZValue);

    int size = trianglesV.size();
    if (size <= 3) { 
        return; 
    }

    if (depth % 3 == 0) { // x
        std::sort(this->trianglesV.begin(), this->trianglesV.end(), XValue);
    }
    else if (depth % 3 == 1) { // y
        std::sort(this->trianglesV.begin(), this->trianglesV.end(), YValue);
    }
    else if(depth % 3 == 2 ) { // z
        std::sort(this->trianglesV.begin(), this->trianglesV.end(), ZValue);
    }

  
    // sort into separate left and right trees
    for (int i = 0; i < this->trianglesV.size(); ++i) {
        if (leftTriSize <= i) {
            rightTri.push_back(this->trianglesV[i]); 
        }
        else { 
            leftTri.push_back(this->trianglesV[i]); 
        }
    }
 
    leftTri.push_back(rightTri.front());
    rightTri.push_back(leftTri.back());
    this->leftB = new BVH(leftTri, depth + 1);
    this->rightB = new BVH(rightTri, depth + 1);
}

float BVH::Intersect(glm::vec3 origin, glm::vec3 ray, float t0, float t1, glm::vec3& normal_)
{
    float t_valueXMin = (minXValue - origin.x) / ray.x;
    float t_ValueXMax = (maxXValue - origin.x) / ray.x;
    float t_ValueYMin = (minYValue - origin.x) / ray.y;
    float t_ValueYMax = (maxYValue - origin.y) / ray.y;
    float t_ValueZMin = (minZValue - origin.z) / ray.z;
    float t_ValueZMax = (maxZValue - origin.z) / ray.z;

    if (t_valueXMin > t_ValueXMax) {
        std::swap(t_valueXMin, t_ValueXMax);
    }
    if (t_ValueYMin > t_ValueYMax) {
        std::swap(t_ValueYMin, t_ValueYMax);
    }
    if ((t_valueXMin > t_ValueYMax) || (t_ValueYMin > t_ValueXMax))
    { 
        return INFINITY; 
    }
    if (t_ValueYMax < t_ValueXMax) {
        t_ValueXMax = t_ValueYMax;
    }
    if (t_ValueYMin > t_valueXMin) {
        t_valueXMin = t_ValueYMin;
    }

    if (t_ValueZMin > t_ValueZMax) {
        std::swap(t_ValueZMin, t_ValueZMax);
    }

    if ((t_valueXMin > t_ValueZMax) || (t_ValueZMin > t_ValueXMax))
    { 
        return INFINITY; 
    }
    if (t_ValueZMax < t_ValueXMax) {
        t_ValueXMax = t_ValueZMax;
    }
    if (t_ValueZMin > t_valueXMin) {
        t_valueXMin = t_ValueZMin;
    }

   // avoids half bunny
    int size = this->trianglesV.size();
    if (size <= 3) {
        float min_t_Value = INFINITY;
        int indexShape = 0;

        for (int i = 0; i < trianglesV.size(); ++i) {

            float intT = trianglesV.at(i)->Intersect(origin, ray, t0, t1, normal_);

            if (intT < min_t_Value) {
                min_t_Value = intT;
                indexShape = i;
            }
        }

        if (min_t_Value != INFINITY) {

            normal_ = trianglesV.at(indexShape)->CalcNormalVector(glm::vec3(1.0f,1.0f,1.0f));
        }

        return min_t_Value;
    }
    // get min value from right and left
    else {
        float rightMinT = rightB->Intersect(origin, ray, t0, t1, normal_);
        float leftMinT = leftB->Intersect(origin, ray, t0, t1, normal_);
       
        float min_t_Value2 = glm::min(leftMinT, rightMinT);
        return min_t_Value2;
    }
}

glm::vec3 BVH::CalcNormalVector(glm::vec3 intersectionPoint)
{
    return glm::vec3(1.0f, 0.0f,0.0f);
}

BVH::~BVH()
{

}