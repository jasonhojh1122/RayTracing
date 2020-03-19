#pragma once

#include "../Include/glm/glm.hpp"
#include "RealRand.hpp"

class Perlin {
public:
    float noise(const glm::vec3& p) const {
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);
        int i = int(4*p.x) & 255;
        int j = int(4*p.y) & 255;
        int k = int(4*p.z) & 255;
        return randFloat[permX[i] ^ permY[j] ^ permZ[k]];
    }

    static float *randFloat;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static float* randomFloatSeires() {
    float *p = new float[256];
    for (int i = 0; i < 256; ++i)
        p[i] = realRand();
    return p;
}

void permute(int *p, int n){
    for (int i = n - 1; i > 0; --i){
        int target = int(realRand()*(i+1));
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

static int* perlinPermute(){
    int *p = new int[256];
    for (int i = 0; i < 256; ++i)
        p[i] = i;
    permute(p, 256);
    return p;        
}

float *Perlin::randFloat = randomFloatSeires();
int *Perlin::permX = perlinPermute();
int *Perlin::permY = perlinPermute();
int *Perlin::permZ = perlinPermute();