#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <fstream>
#include <sstream>
#include <string>
#include "Scene.hpp"


int main(){
    // VideoScene* scene = new VideoScene;
    // scene->render();

    int width = 600;
    int height = 600;
    float aspect = (float)width / (float)height;
    int ns = 8;

    glm::vec3 lookFrom(3.0f, 6.0f, 3.0f);
    glm::vec3 lookAt(0.0f, 0.0f, 0.0f);
    float distToFocus = 10.0f;
    float aperture = 0.0;
    float vFov = 40;
    Camera camera(lookFrom, lookAt, glm::vec3(0.0f, 1.0f, 0.0f), vFov, aspect, aperture, distToFocus, 0.0, 1.0);

    Hitable* world = testImageTexture();

    std::cout << "P3\n" << width << " " << height << "\n255\n";
            
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