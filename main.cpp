#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <limits.h>
#include <random>

#include "Ray.hpp"
#include "HitableList.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Material.hpp"

std::random_device rd;
std::mt19937 gen(rd());

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

Hitable *randomScene(){
    int n = 5000;
    Hitable **list = new Hitable*[n+1];
    list[0] = new Sphere(glm::vec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(glm::vec3(0.5, 0.5, 0.5)));
    int i = 1;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int a = -10; a < 10; ++a){
        for (int b = -10; b < 10; ++b){
            float chooseMat = dis(gen);
            glm::vec3 center(a + 0.9 * dis(gen), 0.2, b + 0.9 * dis(gen));
            if (glm::length(center - glm::vec3(4, 0.2, 0.0)) > 0.9){
                if (chooseMat < 0.75) {
                    // list[i++] = new Sphere(center, 0.2, new Lambertian(glm::vec3(dis(gen) * dis(gen), dis(gen) * dis(gen), dis(gen) * dis(gen))));
                    list[i++] = new MovingSphere(center, center + glm::vec3(0.0, 0.5*dis(gen), 0.0), 0.0, 1.0, 
                                        0.2, new Lambertian(glm::vec3(dis(gen)*dis(gen), dis(gen)*dis(gen), dis(gen)*dis(gen))));
                }
                else if (chooseMat < 0.95){
                    list[i++] = new Sphere(center, 0.2,
                        new Metal(glm::vec3(0.5*(1+dis(gen)), 0.5*(1+dis(gen)), 0.5*(1+dis(gen))), 0.5*dis(gen)));
                }
                else {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(glm::vec3(-4, 1, 0), 1.0, new Lambertian(glm::vec3(0.4, 0.2, 0.1)));
    list[i++] = new Sphere(glm::vec3(4, 1, 0), 1.0, new Metal(glm::vec3(0.7, 0.6, 0.5), 0.0));
    return new HitableList(list, i);
}

int main(){

    int width = 1200;
    int height = 800;
    float aspect = (float)width / (float)height;
    int ns = 15;

    glm::vec3 lookFrom(13.0f, 2.0f, 3.0f);
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    float distToFocus = 10.0;
    float aperture = 0.0;
    float vFov = 20;
    Camera camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), vFov, aspect, aperture, distToFocus, 0.0, 1.0);

    /*
    Hitable *list[5];
    list[0] = new Sphere(glm::vec3(0.0, 0.0, -1.0), 0.5, new Lambertian(glm::vec3(0.8, 0.3, 0.3)));
    list[1] = new Sphere(glm::vec3(0.0, -100.5, -1.0), 100, new Lambertian(glm::vec3(0.8, 0.8, 0.0)));
    list[2] = new Sphere(glm::vec3(1.0, 0.0, -1.0), 0.5, new Metal(glm::vec3(0.8, 0.6, 0.2), 0.3));
    list[3] = new Sphere(glm::vec3(-1.0, 0.0, -1.0), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(glm::vec3(-1.0, 0.0, -1.0), -0.3, new Dielectric(1.8));
    Hitable *world = new HitableList(list, 5);
    */
    Hitable *world = randomScene();

    std::cout << "P3\n" << width << " " << height << "\n255\n";
    
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int j = height - 1; j >= 0; --j){
        for (int i = 0; i < width; ++i){
            
            glm::vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s){
                float u = (float)(i + dis(gen)) / (float)width;
                float v = (float)(j + dis(gen)) / (float)height;
                Ray ray = camera.getRay(u, v, gen);
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

    return 0;
}