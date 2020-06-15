#pragma once
#include <cfloat>
#include "Ray.hpp"
#include "AABB.hpp"
#include "RealRand.hpp"

class Material;

struct hitRecord{
    float t; // ray parameter
    float u, v; // texture coordniate
    glm::vec3 p; //ray point at
    glm::vec3 normal;
    Material *matPtr;
};

class Hitable{
    public:
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const = 0;
    virtual bool boundingBox(float t0, float t1, AABB& box) const = 0;
};

class HitableList : public Hitable{
    public:
    HitableList();
    HitableList(Hitable **l, int s) : list(l), listSize(s) {};
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const; 

    private:
    Hitable **list;
    int listSize;
};

bool HitableList::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    hitRecord tempRec;
    bool hitted = false;
    double closestSoFar = tMax;
    for (int i = 0; i < listSize; ++i){
        if (list[i]->hit(ray, tMin, closestSoFar, tempRec)){
            hitted = true;
            closestSoFar = (double)tempRec.t;
            rec = tempRec;
        }
    }
    return hitted;
}

bool HitableList::boundingBox(float t0, float t1, AABB& box) const{
    if (listSize < 1) return false;
    AABB tmpBox;
    bool first = list[0]->boundingBox(t0, t1, tmpBox);
    if (!first)
        return false;
    else
        box = tmpBox;

    for (int i = 1; i < listSize; ++i){
        if (list[i]->boundingBox(t0, t1, tmpBox))
            box = surroundingBox(box, tmpBox);
        else
            return false;
    }
    return true;
}

class InverseNormal : public Hitable {
public:
    InverseNormal(Hitable* _origin) : origin(_origin) {}
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
        if (origin->hit(ray, tMin, tMax, rec)) {
            rec.normal = -1.0f * rec.normal;
            return true;
        }
        else {
            return false;
        }
    }
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        return origin->boundingBox(t0, t1, box);
    }

private:
    Hitable* origin;
};

class Translate : public Hitable {
public:
    Translate(Hitable* p, glm::vec3 tran) : origin(p), translation(tran) {}
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
        Ray newRay(ray.getBase() - translation, ray.getDirection(), ray.getTime());
        if (origin->hit(newRay, tMin, tMax, rec)) {
            rec.p += translation;
            return true;
        }
        else {
            return false;
        }
    }
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        if (origin->boundingBox(t0, t1, box)) {
            box = AABB(box.getMin() + translation, box.getMax() + translation);
            return true;
        }
        else {
            return false;
        }
    }
    glm::vec3 translation;

private:
    Hitable* origin;
};

class RotateY : public Hitable {
public:
    RotateY(Hitable* _mesh, float angle);
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& _box) const {
        _box = box;
        return hasBox;
    }

private:
    void createBoundingBox();
    void updateCoordinate();
    glm::vec3 rotateVectorCounterClockwise(glm::vec3 vector) const;
    glm::vec3 rotateVectorClockwise(glm::vec3 vector) const;

    Hitable* mesh;
    float sinTheta;
    float cosTheta;
    bool hasBox;
    AABB box;
};

RotateY::RotateY(Hitable* _mesh, float _angle) {
    mesh = _mesh;
    float radians = _angle * M_PI / 360.0;
    sinTheta = sin(radians);
    cosTheta = cos(radians);
    createBoundingBox();
}

void RotateY::createBoundingBox() {
    glm::vec3 minCoord(FLT_MAX, FLT_MAX, FLT_MAX);
    glm::vec3 maxCoord(FLT_MIN, FLT_MIN, FLT_MIN);
    hasBox = mesh->boundingBox(0, 1, box);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                float x = i * box.getMax().x + (i - 1) * box.getMin().x;
                float y = j * box.getMax().y + (j - 1) * box.getMin().y;
                float z = k * box.getMax().z + (k - 1) * box.getMin().z;
                float rotatedX = cosTheta * x + sinTheta * z;
                float rotatedZ = -sinTheta * x + cosTheta * z;
                glm::vec3 newVec = glm::vec3(rotatedX, y, rotatedZ);
                for (int q = 0; q < 3; ++q) {
                    if (newVec[q] > maxCoord[q]) maxCoord[q] = newVec[q];
                    if (newVec[q] < minCoord[q]) minCoord[q] = newVec[q];
                }
            }
        }
    }
    box = AABB(minCoord, maxCoord);
}

bool RotateY::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    glm::vec3 origin = ray.getBase();
    glm::vec3 direction = ray.getDirection();
    origin = rotateVectorClockwise(ray.getBase());
    direction = rotateVectorClockwise(ray.getDirection());
    Ray rotatedRay(origin, direction, ray.getTime());
    if  (mesh->hit(rotatedRay, tMin, tMax, rec)) {
        glm::vec3 p, normal;
        p = rotateVectorCounterClockwise(rec.p);
        normal = rotateVectorCounterClockwise(rec.normal);
        rec.p = p;
        rec.normal = normal;
        return true;
    }
    else {
        return false;
    }
}

glm::vec3 RotateY::rotateVectorCounterClockwise(glm::vec3 vector) const{
    glm::vec3 temp;
    temp.x = cosTheta * vector.x + sinTheta * vector.z;
    temp.z = -sinTheta * vector.x + cosTheta * vector.z;
    temp.y = vector.y;
    return temp;
}
glm::vec3 RotateY::rotateVectorClockwise(glm::vec3 vector) const{
    glm::vec3 temp;
    temp.x = cosTheta * vector.x - sinTheta * vector.z;
    temp.z = sinTheta * vector.x + cosTheta * vector.z;
    temp.y = vector.y;
    return temp;
}

class Volume : public Hitable {
public:
    Volume(Hitable* mesh, float d, Material* mat) {
        boundary = mesh;
        density = d;
        matPtr = mat;
    }
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        return boundary->boundingBox(t0, t1, box);
    }

private:
    Hitable* boundary;
    float density;
    Material* matPtr;
};

bool Volume::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    hitRecord rec1, rec2;
    if (boundary->hit(ray, FLT_MIN, FLT_MAX, rec1)) {
        if (boundary->hit(ray, rec1.t + 0.0001, FLT_MAX, rec2)) {
            if (rec1.t < tMin) 
                rec1.t = tMin;
            if (rec2.t > tMax)
                rec2.t = tMax;
            if (rec1.t >= rec2.t)
                return false;
            if (rec1.t < 0)
                rec1.t = 0;
            float inBoundaryDistance = (rec2.t - rec1.t) * glm::length(ray.getDirection());
            float hitDistance = -(1 / density) * log(realRand());
            if (hitDistance < inBoundaryDistance) {
                rec.t = rec1.t + hitDistance / glm::length(ray.getDirection());
                rec.p = ray.getPointAt(rec.t);
                rec.normal = glm::vec3(1, 0, 0);
                rec.matPtr = matPtr;
                return true;
            }
        }
    }
    return false;
}