#pragma once

#include "Hitable.hpp"

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