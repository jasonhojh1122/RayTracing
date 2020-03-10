#pragma once
#include "Ray.hpp"

struct hitRecord{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
};

class Hitable{
    public:
    virtual bool hit(Ray& ray, float tMin, float tMax, hitRecord& rec) const = 0;
};