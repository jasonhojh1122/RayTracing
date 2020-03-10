#pragma once

#include "Hitable.hpp"

class Sphere : public Hitable{
    public:
    Sphere() {};
    Sphere(glm::vec3 cen, float r) : center(cen), radius(r){};
    virtual bool hit(Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    
    private:
    glm::vec3 center;
    float radius;
};

bool Sphere::hit(Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    glm::vec3 oc = ray.getBase() - center;
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0 * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - radius * radius;
    float det = b*b - 4*a*c;
    if (det > 0){
        float temp = (-b - sqrt(det)) / (2.0f * a);
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = ray.getPointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(det)) / (2.0f * a);
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = ray.getPointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}