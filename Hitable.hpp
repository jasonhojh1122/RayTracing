#pragma once
#include "Ray.hpp"
#include "AABB.hpp"

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
    virtual bool boundingBox(double t0, double t1, AABB& box) const = 0;
};

AABB surroundingBox(AABB box0, AABB box1) {
    glm::vec3 small(ffmin(box0.getMin().x, box1.getMin().x),
                    ffmin(box0.getMin().y, box1.getMin().y),
                    ffmin(box0.getMin().z, box1.getMin().z));
    glm::vec3 big(ffmax(box0.getMax().x, box1.getMax().x),
                  ffmax(box0.getMax().y, box1.getMax().y),
                  ffmax(box0.getMax().z, box1.getMax().z));
    return AABB(small, big);
}