#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "../Include/glm/glm.hpp"
#include "Ray.hpp"
#include "RealRand.hpp"

glm::vec3 randomInUnitDisk(){
    std::uniform_real_distribution<> dis(0.0, 1.0);
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(realRand(), realRand(), 0.0f) - glm::vec3(1.0f, 1.0f, 0.0f);
    } while (glm::dot(p, p) >= 1.0f);
    return p;
}

class Camera{
public:
    Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vUp, float vFov, float aspect, float aperture, float focusDist, double t1, double t2){
        time0 = t1;
        time1 = t2;
        lensRadius = aperture / 2;
        float theta = vFov * M_PI / 180.0f;
        float halfHeight = tan(theta / 2);
        float halfWidth = aspect * halfHeight;
        origin = lookFrom;
        w = glm::normalize(lookFrom - lookAt);
        u = glm::normalize(glm::cross(vUp, w));
        v = glm::cross(w, u);

        lowerLeftCorner = origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist* w;
        horizontal = 2.0f * halfWidth * focusDist * u;
        vertical = 2.0f * halfHeight * focusDist * v;
    }

    Ray getRay(float s, float t){
        glm::vec3 rd = lensRadius * randomInUnitDisk();
        glm::vec3 offset = u * rd.x + v * rd.y;
        std::uniform_real_distribution<> dis(0.0, 1.0);
        double time = time0 + realRand() * (time1 - time0);
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset, time);
    }

private:
    glm::vec3 origin;
    glm::vec3 vertical;
    glm::vec3 horizontal;
    glm::vec3 lowerLeftCorner;
    glm::vec3 u, v, w; // u:horizontal v:vertical
    float lensRadius;
    double time0;
    double time1;
};