#pragma once

#include "../Include/glm/glm.hpp"

class Ray{
public:
    Ray(){};
    Ray(glm::vec3 bas, glm::vec3 dir, double ti = 0.0);
    glm::vec3 getBase() const;
    glm::vec3 getDirection()const;
    glm::vec3 getPointAt(float t) const;
    double getTime() const;

private:
    glm::vec3 base;
    glm::vec3 direction;
    double _time;
};

Ray::Ray(glm::vec3 bas, glm::vec3 dir, double ti) {
    base = bas;
    direction = dir;
    _time = ti;
}
glm::vec3 Ray::getBase() const {
    return base;
}
glm::vec3 Ray::getDirection() const {
    return direction;
}
glm::vec3 Ray::getPointAt(float t) const {
    return base + direction * t;
}
double Ray::getTime() const{
    return _time;
}