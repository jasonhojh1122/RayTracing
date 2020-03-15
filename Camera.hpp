#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include "../Include/glm/glm.hpp"
#include "Ray.hpp"

glm::vec3 randomInUnitDisk(std::mt19937& gen){
    std::uniform_real_distribution<> dis(0.0, 1.0);
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(dis(gen), dis(gen), 0.0f) - glm::vec3(1.0f, 1.0f, 0.0f);
    } while (glm::dot(p, p) >= 1.0f);
    return p;
}

class Camera{
    public:
    Camera(glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 vUp, float vFov, float aspect, float aperture, float focusDist){
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

    Ray getRay(float s, float t, std::mt19937& gen){
        glm::vec3 rd = lensRadius * randomInUnitDisk(gen);
        glm::vec3 offset = u * rd.x + v * rd.y;
        return Ray(origin + offset, lowerLeftCorner + s * horizontal + t * vertical - origin - offset);
    }

    private:
    glm::vec3 origin;
    glm::vec3 vertical;
    glm::vec3 horizontal;
    glm::vec3 lowerLeftCorner;
    glm::vec3 u, v, w; // u:horizontal v:vertical
    float lensRadius;
};