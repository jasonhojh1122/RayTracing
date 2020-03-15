#pragma

#include "../Include/glm/glm.hpp"
#include "Ray.hpp"

class Camera{
    public:
    Camera(){
        lowerLeftCorner = glm::vec3(-2.0, -1.0, -1.0);
        horizontal = glm::vec3(4.0, 0.0, 0.0);
        vertical = glm::vec3(0.0, 2.0, 0.0);
        origin = glm::vec3(0.0, 0.0, 0.0);
    }

    Ray getRay(float u, float v){
        return Ray(origin, lowerLeftCorner + u * horizontal + v * vertical - origin);
    }

    private:
    glm::vec3 lowerLeftCorner;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
};