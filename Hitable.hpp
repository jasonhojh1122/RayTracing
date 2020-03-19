#pragma once

#include "Ray.hpp"
#include "AABB.hpp"

class Material;

AABB surroundingBox(AABB box0, AABB box1) {
    glm::vec3 small(ffmin(box0.getMin().x, box1.getMin().x),
                    ffmin(box0.getMin().y, box1.getMin().y),
                    ffmin(box0.getMin().z, box1.getMin().z));
    glm::vec3 big(ffmax(box0.getMax().x, box1.getMax().x),
                  ffmax(box0.getMax().y, box1.getMax().y),
                  ffmax(box0.getMax().z, box1.getMax().z));
    return AABB(small, big);
}

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

class HitableList : public Hitable{
    public:
    HitableList();
    HitableList(Hitable **l, int s) : list(l), listSize(s) {};
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(double t0, double t1, AABB& box) const; 

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

bool HitableList::boundingBox(double t0, double t1, AABB& box) const{
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