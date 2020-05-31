#pragma once
#include <cmath>
#include "../Include/glm/glm.hpp"
#include "Perlin.hpp"

class Texture {
public:
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

class ConstantTexture : public Texture {
public:
    ConstantTexture(){}
    ConstantTexture(glm::vec3 c) : color(c) {}
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {return color;}

private:
    glm::vec3 color;
};


class CheckerTexture : public Texture {
public:
    CheckerTexture(){}
    CheckerTexture(Texture *tex0, Texture *tex1) : even(tex0), odd(tex1) {}
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
        float sines = sin(10 * p.x) * sin(10 * p.y) * sin(10 * p.z);
        if (sines < 0)
            return odd->value(u, v, p);
        else
            return even->value(u, v, p);
    }

private:
    Texture *even;
    Texture *odd;
};


class NoiseTexture : public Texture {
public:
    NoiseTexture() {}
    NoiseTexture(float sc) : scale(sc) {}
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
        return glm::vec3(1, 1, 1) * noise.noise(scale * p);
    }

    Perlin noise;
    float scale;
};