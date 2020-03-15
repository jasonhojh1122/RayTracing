#pragma once
#include "Ray.hpp"

class Material;

struct hitRecord{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    Material *matPtr;
};

class Hitable{
    public:
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const = 0;
};