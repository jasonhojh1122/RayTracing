#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION
#include <cmath>
#include <iostream>
#include <limits.h>

#include "Ray.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "Rectangles.hpp"

glm::vec3 color(Ray& ray, Hitable *world, int depth){
    hitRecord rec;
    if (world->hit(ray, 0.001, FLT_MAX, rec)){
        Ray scattered;
        glm::vec3 attenuation;
        glm::vec3 emitted = rec.matPtr->emitted(rec.u, rec.v, rec.p);
        if (depth < 50 && rec.matPtr->scatter(ray, rec, attenuation, scattered)){
            return emitted + attenuation * color(scattered, world, depth+1);
        }
        else {
            return emitted;
        }
    }
    else {
        return glm::vec3(0, 0, 0);
        /*
        glm::vec3 unitDirection = glm::normalize(ray.getDirection());
        float t = 0.5f * (unitDirection.y + 1.0f);
        return glm::vec3(1.0, 1.0, 1.0) * (1 - t) + glm::vec3(0.7, 0.5, 1.0) * t;
        */
    }
}

Hitable *randomScene(){
    int n = 1000;
    Hitable **list = new Hitable*[n+1];
    list[0] = new Sphere(glm::vec3(0.0, -1000.0, 0.0), 1000.0, 
                        new Lambertian( 
                            new CheckerTexture(
                                new ConstantTexture(glm::vec3(0.2, 0.3, 0.1)), new ConstantTexture(glm::vec3(0.9, 0.9, 0.9))
                                )
                            )
                        );
    int i = 1;
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int a = -10; a < 10; ++a){
        for (int b = -10; b < 10; ++b){
            float chooseMat = realRand();
            glm::vec3 center(a + 0.9 * realRand(), 0.2, b + 0.9 * realRand());
            if (glm::length(center - glm::vec3(4, 0.2, 0.0)) > 0.9){
                if (chooseMat < 0.75) {
                    // list[i++] = new Sphere(center, 0.2, new Lambertian(glm::vec3(realRand() * realRand(), realRand() * realRand(), realRand() * realRand())));
                    list[i++] = new MovingSphere(center, center + glm::vec3(0.0, 0.5*realRand(), 0.0), 0.0, 1.0, 
                                        0.2, new Lambertian(new ConstantTexture(glm::vec3(realRand()*realRand(), realRand()*realRand(), realRand()*realRand()))));
                }
                else if (chooseMat < 0.95){
                    list[i++] = new Sphere(center, 0.2,
                        new Metal(glm::vec3(0.5*(1+realRand()), 0.5*(1+realRand()), 0.5*(1+realRand())), 0.5*realRand()));
                }
                else {
                    list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                }
            }
        }
    }
    list[i++] = new Sphere(glm::vec3(0, 1, 0), 1.0, new Dielectric(1.5));
    list[i++] = new Sphere(glm::vec3(-4, 1, 0), 1.0, new Lambertian(new ConstantTexture(glm::vec3(0.4, 0.2, 0.1))));
    list[i++] = new Sphere(glm::vec3(4, 1, 0), 1.0, new Metal(glm::vec3(0.7, 0.6, 0.5), 0.0));
    return new HitableList(list, i);
}

Hitable *testPerlin() {
    Texture *perlinTex = new NoiseTexture(8.0f);
    Hitable **list = new Hitable*[2];
    list[0] = new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(perlinTex));
    list[1] = new Sphere(glm::vec3(0, 2, 0), 2, new Lambertian(perlinTex));
    return new HitableList(list, 2);
}

Hitable *twoCheckerSpheres() {
    Texture *checker = new CheckerTexture(new ConstantTexture(glm::vec3(0, 0, 0)), new ConstantTexture(glm::vec3(1, 1, 1)));
    int n = 2;
    Hitable **list = new Hitable*[n];
    list[0] = new Sphere(glm::vec3(0, 10, 0), 10, new Lambertian(checker));
    list[1] = new Sphere(glm::vec3(0, -10, 0), 10, new Lambertian(checker));
    return new HitableList(list, 2);
}

Hitable *testImageTexture() {
    Texture* imageTex = new ImageTexture("texture/earthmap.jpg");
    Hitable** list = new Hitable*[2];
    list[0] = new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(new ConstantTexture(glm::vec3(0.8, 0.8, 0.8))));
    list[1] = new Sphere(glm::vec3(0, 2, 0), 2, new Lambertian(imageTex));
    return new HitableList(list, 2);
}

Hitable *testLighting() {
    Texture *perlinTex = new NoiseTexture(8.0f);
    Hitable **list = new Hitable*[3];
    list[0] = new Sphere(glm::vec3(0, -1000, 0), 1000, new Lambertian(perlinTex));
    list[1] = new Sphere(glm::vec3(0, 2, 0), 2, new Lambertian(perlinTex));
    list[2] = new XYRect(-2, 2, 2, 6, -4, new DiffuseLight(new ConstantTexture(glm::vec3(4, 4, 4))));
    return new HitableList(list, 3);
}

Hitable *cornellBox() {
    Texture *imageTex = new ImageTexture("texture/earthmap.jpg");
    Hitable **list = new Hitable*[8];
    Material* red = new Lambertian(new ConstantTexture(glm::vec3(0.65, 0.05, 0.05)));
    Material* green = new Lambertian(new ConstantTexture(glm::vec3(0.12, 0.45, 0.15)));
    Material* white = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));
    Material* gray = new Lambertian(new ConstantTexture(glm::vec3(0.21, 0.21, 0.21)));
    Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(5.0, 5.0, 5.0)));

    list[0] = new InverseNormal(new YZRect(0, 555, 0, 555, 555, green)); //left
    list[1] = new YZRect(0, 555, 0, 555, 0, red); //right
    
    list[2] = new InverseNormal(new XZRect(0, 555, 0, 555, 555, white)); //top
    list[3] = new XZRect(0, 555, 0, 555, 0, white); //down
    
    //back
    list[4] = new InverseNormal(new XYRect(0, 555, 0, 555, 555, white));

    //light
    list[5] = new InverseNormal(new XZRect(113, 443, 127, 432, 554, light));

    //box
    Hitable* box1 = new Translate(new RotateY (
                                    new Box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), 
                                    -18
                            ), 
                            glm::vec3(130, 1, 65));
    
    Hitable* box2 = new Translate(new RotateY (
                                    new Box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white),
                                    15
                            ),
                            glm::vec3(265, 1, 295));

    list[6] = new Volume(box1, 0.01f, new Isotropic(new ConstantTexture(glm::vec3(1.0, 1.0, 1.0))));
    list[7] = new Volume(box2, 0.01f, new Isotropic(new ConstantTexture(glm::vec3(0.0, 0.0, 0.0))));
    
    return new HitableList(list, 8);
}

int main(){
    int width = 600;
    int height = 600;
    float aspect = (float)width / (float)height;
    int ns = 16;

    glm::vec3 lookFrom(278.0f, 278.0f, -800.0f);
    glm::vec3 lookAt(278.0f, 278.0f, 0.0f);
    float distToFocus = 10.0f;
    float aperture = 0.0;
    float vFov = 40;
    Camera camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), vFov, aspect, aperture, distToFocus, 0.0, 1.0);

    Hitable *world = cornellBox();

    std::cout << "P3\n" << width << " " << height << "\n255\n";
    
    std::uniform_real_distribution<> dis(0.0, 1.0);
    for (int j = height - 1; j >= 0; --j){
        for (int i = 0; i < width; ++i){
            
            glm::vec3 col(0.0, 0.0, 0.0);
            for (int s = 0; s < ns; ++s){
                float u = (float)(i + realRand()) / (float)width;
                float v = (float)(j + realRand()) / (float)height;
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

    return 0;
}