#pragma once

#include "Hitable.hpp"

class HitableList : public Hitable{
    public:
    HitableList();
    HitableList(Hitable **l, int s) : list(l), listSize(s) {};
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;

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