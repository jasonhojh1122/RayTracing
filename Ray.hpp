#pragma once
#include "../Include/glm/glm.hpp"

class Ray{
    public:
    Ray(glm::vec3 bas, glm::vec3 dir) {base = bas; direction = dir;};
    glm::vec3 getBase() {return base;};
    glm::vec3 getDirection() {return direction;};
    glm::vec3 getPointAt(float t) {return base + direction * t;};

    private:
    glm::vec3 base;
    glm::vec3 direction;
};