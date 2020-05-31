#pragma once

#include <cstdlib>
#include <algorithm>
#include "../Include/glm/glm.hpp"
#include "RealRand.hpp"

inline float linearInterp(float c[2][2][2], float u, float v, float w) {
    float accum = 0.0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                accum += (i*u + (1-i)*(1-u))*
                         (j*v + (1-j)*(1-v))*
                         (k*w + (1-k)*(1-w))*c[i][j][k];
            }
        }
    }
    return accum;
}

inline float fade(float t) {
    // return t*t*(3-2*t);
    return t * t * t * (t * (t * 6 - 15) + 10);
}

class Perlin {
public:
    float noise(const glm::vec3& p) const {
        float u = p.x - floor(p.x);
        float v = p.y - floor(p.y);
        float w = p.z - floor(p.z);
        u = fade(u);
        v = fade(v);
        w = fade(w);
        int i = floor(p.x);
        int j = floor(p.y);
        int k = floor(p.z);
        float c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = randFloat[permX[(i+di)&255] ^ permY[(j+dj)&255] ^ permZ[(k+dk)&255]];
                }
            }
        }
        return linearInterp(c, u, v, w);
    }

    static float* randFloat;
    static int *permX;
    static int *permY;
    static int *permZ;
};

static float *randomFloat() {
    float *p = new float[256];
    for (int i = 0; i < 256; ++i)
        p[i] = realRand();
    return p;
}

void permute(int *p, int n){
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(p, p+n, g);
}

static int* perlinPermute(){
    int *p = new int[256];
    for (int i = 0; i < 256; ++i)
        p[i] = i;
    permute(p, 256);
    return p;
}

float *Perlin::randFloat = randomFloat();
int *Perlin::permX = perlinPermute();
int *Perlin::permY = perlinPermute();
int *Perlin::permZ = perlinPermute();