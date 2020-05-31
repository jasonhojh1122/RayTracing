#pragma once

#include <algorithm>
#include "Ray.hpp"

inline float ffmin(float a, float b){ return a < b ? a : b;}
inline float ffmax(float a, float b){ return a > b ? a : b;}

class AABB {
public:
    AABB(){}
    AABB(const glm::vec3& a, const glm::vec3& b) {_min = a; _max = b;}

    glm::vec3 getMin() const{return _min;}
    glm::vec3 getMax() const{return _max;}

    bool hit(const Ray& ray, float tmin, float tmax) const {
        for (int a = 0; a < 3; ++a){
            float dirA = 1 / ray.getDirection()[a];
            float t0 = (_min[a] - ray.getBase()[a]) * dirA;
            float t1 = (_min[a] - ray.getBase()[a]) * dirA;
            if (dirA < 0.0f) {
                std::swap(t0, t1);
            }

            tmin = ffmax(t0, tmin);
            tmax = ffmin(t1, tmax);

            if (tmax <= tmin) {
                return false;
            }
        }
        return true;
    }

private:
    glm::vec3 _min, _max;
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