#include <iostream>
#include <cmath>
#include <limits.h>
#include <random>

#include "Ray.hpp"
#include "HitableList.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Material.hpp"

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);



glm::vec3 color(Ray& ray, Hitable *world, int depth){
    hitRecord rec;
    if (world->hit(ray, 0.001, FLT_MAX, rec)){
        Ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && rec.matPtr->scatter(ray, rec, attenuation, scattered, gen)){
            return attenuation * color(scattered, world, depth+1);
        }
        else {
            return glm::vec3(0.0, 0.0, 0.0);
        }
    }
    else {
        glm::vec3 unitDirection = glm::normalize(ray.getDirection());
        float t = 0.5f * (unitDirection.y + 1.0f);
        return glm::vec3(1.0, 1.0, 1.0) * (1 - t) + glm::vec3(0.7, 0.5, 1.0) * t;
    }
}

int main(){

    int multiple = 200;
    int width = 2 * multiple;
    int height = 1 * multiple;
    int ns = 50;

    Camera camera;

    Hitable *list[5];
    list[0] = new Sphere(glm::vec3(0.0, 0.0, -1.0), 0.5, new Lambertian(glm::vec3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(glm::vec3(0.0, -100.5, -1.0), 100, new Lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(glm::vec3(1.0, 0.0, -1.0), 0.5, new Metal(glm::vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new Sphere(glm::vec3(-1.0, 0.0, -1.0), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(glm::vec3(-1.0, 0.0, -1.0), -0.45, new Dielectric(1.5));
    Hitable *world = new HitableList(list, 5);
    
    std::cout << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j){
        for (int i = 0; i < width; ++i){
            
            glm::vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s){
                float u = (float)(i + dis(gen)) / (float)width;
                float v = (float)(j + dis(gen)) / (float)height;
                Ray ray = camera.getRay(u, v);
                col += color(ray, world, 0);
            }
            
            col /= (float)ns;
            col = glm::vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
            int outR = (int)(255.99 * col[0]);
            int outG = (int)(255.99 * col[1]);
            int outB = (int)(255.99 * col[2]);
            
            std::cout << outR << ' ' << outG << ' ' << outB << ' ';
        }
        std::cout << '\n';
    }
}
