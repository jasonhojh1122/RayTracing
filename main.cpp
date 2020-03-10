#include <iostream>
#include <cmath>
#include "Ray.hpp"

float hitSphere(glm::vec3 center, float radius, Ray ray){
    glm::vec3 oc = ray.getBase() - center;
    float a = glm::dot(ray.getDirection(), ray.getDirection());
    float b = 2.0 * glm::dot(oc, ray.getDirection());
    float c = glm::dot(oc, oc) - radius * radius;
    float det = b*b - 4*a*c;
    if (det < 0)
        return -1;
    else
        return (-b - sqrt(det)) / (2 * a);
}

glm::vec3 color(Ray& ray){
    glm::vec3 sphereOrigin(0.0, 0.0, -1.0);
    float t = hitSphere(sphereOrigin, 0.5, ray);
    if (t >= 0.0){
        glm::vec3 normal = ray.getPointAt(t) - sphereOrigin;
        normal = glm::normalize(normal);
        normal += glm::vec3(1.0, 1.0, 1.0);
        return 0.5f * normal;
    }
    glm::vec3 unittDirection = glm::normalize(ray.getDirection());
    t = 0.5f * (unittDirection.y + 1.0f);
    return glm::vec3(1.0, 1.0, 1.0) * (1 - t) + glm::vec3(0.7, 0.5, 1.0) * t;
}

int main(){
    int width = 200;
    int height = 100;

    glm::vec3 lowerLeftCorner(-2.0, -1.0, -1.0);
    glm::vec3 horizontal(4.0, 0.0, 0.0);
    glm::vec3 vertical(0.0, 2.0, 0.0);
    glm::vec3 origin(0.0, 0.0, 0.0);

    std::cout << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j){
        for (int i = 0; i < width; ++i){
            float u = (float)i / (float)width;
            float v = (float)j / (float)height;
            Ray r(origin, lowerLeftCorner + horizontal * u + vertical * v);
            glm::vec3 col = color(r);
            int outR = (int)(255.99 * col[0]);
            int outG = (int)(255.99 * col[1]);
            int outB = (int)(255.99 * col[2]);
            
            std::cout << outR << ' ' << outG << ' ' << outB << ' ';
        }
        std::cout << '\n';
    }
}
