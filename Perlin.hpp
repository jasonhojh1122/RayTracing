#pragma once

#include "../Include/glm/glm.hpp"
#include "RealRand.hpp"

class Perlin {
public:
    float noise(const glm::vec3& p) const {
        
    }
};

static float* randomFloatSeires() {
    float *p = new float[256];
    for (int i = 0; i < 256; ++i)
        p[i] = realRand();
    return p;
}

