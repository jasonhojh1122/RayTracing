#pragma once
#include <cmath>
#include <algorithm>
#include "../Include/glm/glm.hpp"
#include "Perlin.hpp"
#include "../Include/stb_master/stb_image.h"

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
        // return glm::vec3(1, 1, 1) * 0.5f * (1.0f + (float)noise.noise(scale * p));
        // return glm::vec3(1, 1, 1) * 0.5f * (float)(1 + noise.turb(scale * p));
        return glm::vec3(1, 1, 1) * 0.5f * (float)(1 + sin(scale * p.z + 10*noise.turb(p)));
    }

    Perlin noise;
    float scale;
};

class ImageTexture : public Texture {
public:
    ImageTexture(std::string path) { loadTexture(path); }

    void loadTexture(std::string path) {
        int texChannels;
        pixels = stbi_load(path.c_str(), &width, &height, &texChannels, STBI_rgb_alpha);
        if (!pixels)
            throw std::runtime_error("Failed to load texture image file.");
    }

    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const {
        int i = u * width;
        int j = (1-v) * height - 0.0001;
        
        if (i < 0) i = 0;
        if (j < 0) j = 0;
        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

        float r = (int)pixels[j * BYTES_PER_PIXEL * width + i * BYTES_PER_PIXEL] / 255.0f;
        float g = (int)pixels[j * BYTES_PER_PIXEL * width + i * BYTES_PER_PIXEL + 1] / 255.0f;
        float b = (int)pixels[j * BYTES_PER_PIXEL * width + i * BYTES_PER_PIXEL + 2] / 255.0f;
        
        return glm::vec3(r, g, b);
    }

private:
    int BYTES_PER_PIXEL = 4;

    int width;
    int height;
    stbi_uc* pixels;
};