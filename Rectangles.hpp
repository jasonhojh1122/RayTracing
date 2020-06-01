#pragma once
#include "Hitable.hpp"

class Material;

class XYRect : public Hitable {
public:
    XYRect() {}
    XYRect(float x0_in, float x1_in, float y0_in, float y1_in, float z_in, Material* mat) :
    x0(x0_in), x1(x1_in), y0(y0_in), y1(y1_in), z(z_in), matPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        box = AABB(glm::vec3(x0, y0, z - 0.0001), glm::vec3(x1, y1, z + 0.0001));
        return true;
    }

private:
    float x0;
    float x1;
    float y0;
    float y1;
    float z;
    Material* matPtr;
};

bool XYRect::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    float t = (z - ray.getBase().z) / ray.getDirection().z;
    if (t < tMin || t > tMax) {
        return false;
    }
    float x = ray.getBase().x + t * ray.getDirection().x;
    float y = ray.getBase().y + t * ray.getDirection().y;
    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.matPtr = matPtr;
    rec.p = ray.getPointAt(t);
    rec.normal = glm::vec3(0, 0, 1);
    return true;
}