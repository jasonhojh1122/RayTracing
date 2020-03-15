#pragma once

#include "../Include/glm/glm.hpp"

class Ray{
    public:
    Ray(){};
    Ray(glm::vec3 bas, glm::vec3 dir) {base = bas; direction = dir;};
    glm::vec3 getBase() const {return base;} ;
    glm::vec3 getDirection() const {return direction;};
    glm::vec3 getPointAt(float t) const {return base + direction * t;};

    private:
    glm::vec3 base;
    glm::vec3 direction;
};