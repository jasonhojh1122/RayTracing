#pragma once

#include "Hitable.hpp"

class Sphere : public Hitable{
public:
    Sphere() {};
    Sphere(glm::vec3 cen, float r, Material *mat) : center(cen), radius(r), matPtr(mat) {};
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, AABB& box) const;

private:
    glm::vec3 center;
    float radius;
    Material *matPtr;
};

bool Sphere::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const{
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
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrt(det)) / (2.0f * a);
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = ray.getPointAt(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
    }
    return false;
}

bool Sphere::boundingBox(double t0, double t1, AABB& box) const {
    box = AABB(center - glm::vec3(radius, radius, radius), center + glm::vec3(radius, radius, radius));
    return true;
}


class MovingSphere : public Hitable {
public:
    MovingSphere(){};
    MovingSphere(glm::vec3 cen0, glm::vec3 cen1, double t0, double t1, float r, Material *m){
        center0 = cen0; center1 = cen1;
        time0 = t0; time1 = t1;
        radius = r;
        matPtr = m;
    }
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, AABB& box) const;
    glm::vec3 getCenter(double time) const;

private:
    glm::vec3 center0, center1;
    float radius;
    double time0, time1;
    Material*matPtr;
};

bool MovingSphere::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    glm::vec3 oc = ray.getBase() - getCenter(ray.getTime());
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0 * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - radius * radius;
    float det = b*b - 4*a*c;
    
    if (det > 0){
        float temp = (-b - sqrt(det)) / (2.0f * a);
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = ray.getPointAt(rec.t);
            rec.normal = (rec.p - getCenter(ray.getTime())) / radius;
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrt(det)) / (2.0f * a);
        if (temp < tMax && temp > tMin){
            rec.t = temp;
            rec.p = ray.getPointAt(rec.t);
            rec.normal = (rec.p - getCenter(ray.getTime())) / radius;
            rec.matPtr = matPtr;
            return true;
        }
    }
    return false;
}

glm::vec3 MovingSphere::getCenter(double time) const{
    return center0 + (float)((time - time0) / (time1 - time0)) * (center1 - center0);
}

bool MovingSphere::boundingBox(double t0, double t1, AABB& box) const {
    AABB box0 = AABB(center0 - glm::vec3(radius, radius, radius), center0 + glm::vec3(radius, radius, radius));
    AABB box1 = AABB(center1 - glm::vec3(radius, radius, radius), center1 + glm::vec3(radius, radius, radius));
    box = surroundingBox(box0, box1);
    return true;
}