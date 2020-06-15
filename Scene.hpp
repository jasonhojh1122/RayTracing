#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <limits.h>
#include <string>

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
        std::cerr << "HIT\n";
    }
    else {
        return glm::vec3(0, 0, 0);
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
    Hitable **list = new Hitable*[20];
    Material* red = new Lambertian(new ConstantTexture(glm::vec3(0.65, 0.05, 0.05)));
    Material* green = new Lambertian(new ConstantTexture(glm::vec3(0.12, 0.45, 0.15)));
    Material* white = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));
    Material* gray = new Lambertian(new ConstantTexture(glm::vec3(0.21, 0.21, 0.21)));
    Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(15.0, 15.0, 15.0)));
    Material* greenLight = new DiffuseLight(new ConstantTexture(glm::vec3(1.0, 3.5, 1.25)));
    Material* redLight = new DiffuseLight(new ConstantTexture(glm::vec3(3.5, 1.19, 1.19)));
    int i = 0;
    list[i++] = new InverseNormal(new YZRect(0, 555, 0, 555, 555, green)); //left
    list[i++] = new YZRect(0, 555, 0, 555, 0, red); //right
    
    list[i++] = new InverseNormal(new XZRect(0, 555, 0, 555, 555, white)); //top
    list[i++] = new XZRect(0, 555, 0, 555, 0, white); //down
    
    //back
    list[i++] = new InverseNormal(new XYRect(0, 555, 0, 555, 555, white));

    //light
    list[i++] = new InverseNormal(new XZRect(213, 343, 227, 332, 554, light));

    //box
    /*
    list[6] = new Sphere(glm::vec3(425, 81, 250), 80.0, new Lambertian(new ConstantTexture(glm::vec3(0.4, 0.2, 0.1))));
    list[7] = new Sphere(glm::vec3(130, 81, 250), 80.0, new Metal(glm::vec3(0.7, 0.6, 0.5), 0.0));
    list[8] = new Sphere(glm::vec3(278, 81, 150), 80.0, new Dielectric(1.5));
    list[9] = new YZRect(10, 160, 125, 375, 1, new Metal(glm::vec3(0.8, 0.8, 0.8), 0.0));

    list[10] = new YZRect(200, 210, 175, 325, 1, redLight);
    list[11] = new InverseNormal(new YZRect(200, 210, 175, 325, 554, greenLight));
    */
    
    list[i++] = new Translate(new RotateY (
                                    new Box(glm::vec3(0, 0, 0), glm::vec3(165, 165, 165), white), 
                                    -18
                            ), 
                            glm::vec3(130, 1, 65));
    
    list[i++] = new Translate(new RotateY (
                                    new Box(glm::vec3(0, 0, 0), glm::vec3(165, 330, 165), white),
                                    15
                            ),
                            glm::vec3(265, 1, 295));
    /*
    list[6] = new Volume(box1, 0.01f, new Isotropic(new ConstantTexture(glm::vec3(1.0, 1.0, 1.0))));
    list[7] = new Volume(box2, 0.01f, new Isotropic(new ConstantTexture(glm::vec3(0.0, 0.0, 0.0))));
    */
    return new HitableList(list, i);
}



#define boxNum 10

class VideoScene {
public:
    ~VideoScene();
    VideoScene();
    void render();
    Hitable* getScene(int frame);

private:
    int FRAME_PER_SECOND = 24;
    int VIDEO_LENGTH = 10;
    int sphereRadius = 4;
    float g = 9.8f;
    float dt = 1.0f / (float)FRAME_PER_SECOND;

    int boxHeight[boxNum][boxNum];
    float sphereVelocity[boxNum][boxNum];
    float sphereHeight[boxNum][boxNum];
    Sphere** spheres;
    Hitable **meshes;
    Material* floor = new Lambertian(new ConstantTexture(glm::vec3(0.73, 0.73, 0.73)));
    Material* light = new DiffuseLight(new ConstantTexture(glm::vec3(5.0, 5.0, 5.0)));
};

VideoScene::VideoScene() {
    meshes = new Hitable*[boxNum*boxNum*2 + 10];
    spheres = new Sphere*[boxNum*boxNum];

    for (int i = 0; i < boxNum; ++i) {
        for (int j = 0; j < boxNum; ++j) {
            boxHeight[i][j] = int(realRand()*10);
            meshes[i*boxNum + j] = new Translate(new Box(glm::vec3(0, 0, 0), glm::vec3(10, 10, 10), floor), 
                                                glm::vec3(i * 10, boxHeight[i][j], j * 10)
                                        );

            sphereVelocity[i][j] = 14.0f * realRand() - 7.0f;
            sphereHeight[i][j] = boxHeight[i][j] + 12.0f + sphereRadius + realRand() * 20.0f;

            float matChooser = realRand();
            Material* mat;
            if (matChooser <= 0.25) { //metal
                mat = new Metal(glm::vec3(0.5*(1+realRand()), 0.5*(1+realRand()), 0.5*(1+realRand())), 0.5*realRand());
            }
            else if (matChooser <= 0.85) { //lambertian
                mat = new Lambertian(new ConstantTexture(glm::vec3(realRand()*realRand(), realRand()*realRand(), realRand()*realRand())));
            }
            else { //dielectric
                mat = new Dielectric(1.5 + realRand()*realRand());
            }
            spheres[i*boxNum + j] = new Sphere(glm::vec3(0, 0, 0), sphereRadius, mat);
            meshes[boxNum*boxNum + i*boxNum + j] = new Translate( spheres[i*boxNum + j],
                                                          glm::vec3(i * 10 + 5, sphereHeight[i][j], j * 10 + 5)  
                                                    );
        }
    }
    meshes[200] = new InverseNormal(new XZRect(0, 100, 0, 100, 150, light));
    meshes[201] = new XYRect(0, 100, 0, 100, -150, light);
}

Hitable* VideoScene::getScene(int frame) {
    for (int i = 0; i < boxNum; ++i) {
        for (int j = 0; j < boxNum; ++j) {
            sphereVelocity[i][j] -= g * dt;
            sphereHeight[i][j] += sphereVelocity[i][j] * dt;

            if (sphereHeight[i][j] < boxHeight[i][j] + 10.0f + sphereRadius) {
                sphereVelocity[i][j] = -sphereVelocity[i][j];
            }

            delete meshes[boxNum*boxNum + i*boxNum + j];
            meshes[boxNum*boxNum + i*boxNum + j] = new Translate( spheres[i*boxNum + j],
                                                          glm::vec3(i * 10 + 5, sphereHeight[i][j], j * 10 + 5)  
                                                    );
        }
    }
    return new HitableList(meshes, boxNum*boxNum*2+2);
}

void VideoScene::render() {
    int width = 800;
    int height = 450;
    float aspect = (float)width / (float)height;
    int ns = 128;
    glm::vec3 lookFrom(80.0f, 40.0f, -50.0f);
    glm::vec3 lookAt(50.0f, 5.0f, 50.0f);
    float distToFocus = 10.0f;
    float aperture = 0.0;
    float vFov = 40;
    Camera camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), vFov, aspect, aperture, distToFocus, 0.0, 1.0);

    for (int frame = 0; frame < FRAME_PER_SECOND * VIDEO_LENGTH; ++frame) {
        
        Hitable *world = getScene(frame);

        std::ostringstream fileNameStream;
        std::string fileName;
        fileNameStream << "video_" << frame << ".ppm";
        fileName = fileNameStream.str();

        std::ofstream file;
        file.open(fileName.c_str());

        file << "P3\n" << width << " " << height << "\n255\n";
        std::cout << "frame: " << frame << "\n";

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
                outR = std::clamp(outR, 0, 255);
                outG = std::clamp(outG, 0, 255);
                outB = std::clamp(outB, 0, 255);
                file << outR << ' ' << outG << ' ' << outB << ' ';
            }
            file << '\n';
        }
        file.close();
    }
}
