#include <iostream>
#include "Ray.hpp"

glm::vec3 color(Ray r){
    glm::vec3 unittDirection = glm::normalize(r.getDirection());
    float t = 0.5f * (unittDirection.y + 1.0f);
    return glm::vec3(1.0, 1.0, 1.0) * (1 - t) + glm::vec3(0.7, 0.5, 1.0) * t;
}

int main(){
    int width = 200;
    int height = 100;

    std::cout << "P3\n" << width << " " << height << "\n255\n";

    for (int i = width - 1; i >= 0; --i){
        for (int j = 0; j < height; ++j){
            float 
        }
    }

}
