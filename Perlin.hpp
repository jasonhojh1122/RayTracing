#pragma once

#include <cstdlib>
#include <vector>
#include <algorithm>
#include "../Include/glm/glm.hpp"
#include "RealRand.hpp"

inline double fade(double t) {
    // return t*t*(3-2*t);
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double linearInterp(glm::vec3 c[2][2][2], double u, double v, double w) {
    double uu = fade(u);
    double vv = fade(v);
    double ww = fade(w);
    double accum = 0.0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                glm::vec3 weightVector(u - (double)i, v - (double)j, w - (double)k);
                accum += (i*uu + (1-i)*(1-uu))*
                         (j*vv + (1-j)*(1-vv))*
                         (k*ww + (1-k)*(1-ww))* glm::dot<3, double, glm::qualifier::highp>(c[i][j][k], weightVector);
            }
        }
    }
    return accum;
}

class Perlin {
public:
    double noise(const glm::vec3& p) const {
        double u = p.x - floor(p.x);
        double v = p.y - floor(p.y);
        double w = p.z - floor(p.z);
        int i = floor(p.x);
        int j = floor(p.y);
        int k = floor(p.z);
        glm::vec3 c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = randVector[permX[(i+di)&255] ^ permY[(j+dj)&255] ^ permZ[(k+dk)&255]];
                }
            }
        }
        return linearInterp(c, u, v, w);
    }

    double turb(const glm::vec3& p, int depth = 7) const {
        double accum = 0.0;
        glm::vec3 temp_p = p;
        double weight = 1.0;
        for (int i = 0; i < depth; ++i) {
            accum += weight * noise(temp_p);
            weight *= 0.5;
            temp_p *= 2.0;
        }
        return fabs(accum);
    }

    static std::vector<glm::vec3> randVector;
    static int* permX;
    static int* permY;
    static int* permZ;
};

static std::vector<glm::vec3> randomVector() {
    std::vector<glm::vec3> p;
    p.resize(256);
    for (int i = 0; i < 256; ++i) {
        p[i] = glm::vec3(2.0 * realRand() - 1.0, 2.0 * realRand() - 1.0, 2.0 * realRand() - 1.0);
        p[i] = glm::normalize(p[i]);
    }
    return p;
}

static int* perlinPermute(){
    int *p = new int[256];
    for (int i = 0; i < 256; ++i)
        p[i] = i;
    std::shuffle(p, p+256, gen);
    return p;
}

std::vector<glm::vec3> Perlin::randVector = randomVector();
int* Perlin::permX = perlinPermute();
int* Perlin::permY = perlinPermute();
int* Perlin::permZ = perlinPermute();