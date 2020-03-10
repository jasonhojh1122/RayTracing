#pragma once

#include "Hitable.hpp"

class HitableList : public Hitable{
    public:
    HitableList();
    HitableList(Hitable **l, int s) : list(l), listSize(s) {};
    virtual bool hit(Ray& ray, float tMin, float tMax, hitRecord& rec) const;

    private:
    Hitable **list;
    int listSize;
};

bool HitableList::hit(Ray& ray, float tMin, float tMax, hitRecord& rec) const{
    hitRecord tempRec;
    bool hitted = false;
    double cloest = tMax;
    for (int i = 0; i < listSize; ++i){
        if (list[i]->hit(ray, tMin, tMax, tempRec)){
            hitted = true;
            cloest = (double)tempRec.t;
            rec = tempRec;
        }
    }
    return hitted;
}