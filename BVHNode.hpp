#pragma once

#include <stdexcept>
#include "Hitable.hpp"
#include "RealRand.hpp"

class BVHNode : public Hitable {
public:
    BVHNode() {}
    BVHNode(Hitable **l, int n, float t0, float t1);
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const;
    Hitable *left;
    Hitable *right;
    AABB box;
};

int boxXCompare(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable **)a;
    Hitable *bh = *(Hitable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        throw std::runtime_error("no bounding box for BVH construction.");
    if (boxLeft.getMin().x - boxRight.getMin().x < 0.0)
        return -1;
    else
        return 1;
}

int boxYCompare(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable **)a;
    Hitable *bh = *(Hitable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        throw std::runtime_error("no bounding box for BVH construction.");
    if (boxLeft.getMin().y - boxRight.getMin().y < 0.0)
        return -1;
    else
        return 1;
}

int boxZCompare(const void *a, const void *b){
    AABB boxLeft, boxRight;
    Hitable *ah = *(Hitable **)a;
    Hitable *bh = *(Hitable **)b;
    if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight))
        throw std::runtime_error("no bounding box for BVH construction.");
    if (boxLeft.getMin().z - boxRight.getMin().z < 0.0)
        return -1;
    else
        return 1;
}

BVHNode::BVHNode(Hitable **l, int n, float t0, float t1){
    int axis = int(3.0 * realRand());
    if (axis == 0) {
        qsort(l, n, sizeof(Hitable*), boxXCompare);
    }
    else if (axis == 1) {
        qsort(l, n, sizeof(Hitable*), boxYCompare);
    }        
    else {
        qsort(l, n, sizeof(Hitable*), boxZCompare);
    }

    if (n == 1) {
        left = right = l[0];
    }
    else if (n == 2){
        left = l[0];
        right = l[1];
    }
    else {
        left = new BVHNode(l, n/2, t0, t1);
        right = new BVHNode(l + n/2, n - n/2, t0, t1);
    }
    AABB leftBox, rightBox;
    if (!left->boundingBox(t0, t1, leftBox) || !right->boundingBox(t0, t1, rightBox))
        throw std::runtime_error("no bounding box for child BVH node in BVH construction.");
    box = surroundingBox(leftBox, rightBox);
}

bool BVHNode::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    if (box.hit(ray, tMin, tMax)) {
        hitRecord recLeft, recRight;
        bool hitLeft, hitRight;
        hitLeft  = left->hit(ray, tMin, tMax, recLeft);
        hitRight = right->hit(ray, tMin, tMax, recRight);
        if (hitLeft && hitRight) {
            if (recLeft.t < recRight.t)
                rec = recLeft;
            else
                rec = recRight;
            return true;
        }
        else if (hitLeft) {
            rec = recLeft;
            return true;
        }
        else if (hitRight) {
            rec = recRight;
            return true;
        }
        else return false;
    }
    else return false;
}

bool BVHNode::boundingBox(float t0, float t1, AABB& b) const{
    b = box;
    return true;
}

