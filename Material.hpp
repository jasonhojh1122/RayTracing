#pragma once

#include <cmath>
#include <random>
#include "Hitable.hpp"
#include "Ray.hpp"
#include "RealRand.hpp"
#include "Texture.hpp"

glm::vec3 randomPointInUnitSphere(){
    std::uniform_real_distribution<> dis(0.0, 1.0);
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(realRand(), realRand(), realRand()) - glm::vec3(1.0, 1.0, 1.0);
    } while(glm::length(p) >= 1.0);
    return p;
}

glm::vec3 reflect(glm::vec3 inDir, glm::vec3 normal){
    return inDir - 2.0f * glm::dot(inDir, normal) * normal;
}

bool refract(glm::vec3 inDir, glm::vec3 normal, float refractiveRatio, glm::vec3& refracted){
    inDir = glm::normalize(inDir);
    float cosTheta1 = glm::dot(inDir, -1.0f * normal);
    float cosTheta2Square = 1.0f - refractiveRatio * refractiveRatio * (1 - cosTheta1 * cosTheta1);
    if (cosTheta2Square > 0.0f){
        refracted = refractiveRatio * inDir + (refractiveRatio * cosTheta1 - (float)sqrt(cosTheta2Square)) * normal;
        return true;
    }
    else {
        return false;
    }
}

float schlick(float cosine, float refIdx){
    float r0 = (1.0f - refIdx) / (1.0f + refIdx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow(1 - cosine, 5);
}

class Material{
public:
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered) = 0;
    virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const { return glm::vec3(0, 0, 0); }
};

class Lambertian : public Material{
public:
    Lambertian(Texture *a){albedo = a;};
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered) override {
        glm::vec3 target = rec.p + rec.normal + randomPointInUnitSphere();
        scattered = Ray(rec.p, target - rec.p, rIn.getTime());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    Texture *albedo;
};

class Metal : public Material{
public:
    Metal(glm::vec3 a, float r) {albedo = a; fuzz = r;};
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered) override{
        glm::vec3 reflected = reflect(glm::normalize(rIn.getDirection()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * randomPointInUnitSphere(), rIn.getTime());
        attenuation = albedo;
        return (glm::dot(scattered.getDirection(), rec.normal) > 0);
    }

private:
    glm::vec3 albedo;
    float fuzz;
};

class Dielectric : public Material{
public:
    Dielectric(float ri) : refIdx(ri){};
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered){
        glm::vec3 outNormal;
        glm::vec3 reflected = reflect(rIn.getDirection(), rec.normal);
        glm::vec3 refracted;
        float refractiveRatio;
        float reflectProb;
        float cosine;
        attenuation = glm::vec3(1.0f, 1.0f, 1.0f);

        if (glm::dot(rIn.getDirection(), rec.normal) > 0){
            outNormal = -1.0f * rec.normal;
            refractiveRatio = refIdx;
            cosine = glm::dot(rIn.getDirection(), rec.normal) / glm::length(rIn.getDirection());
            cosine = sqrt(1 - refIdx * refIdx * (1 - cosine * cosine));
        }
        else {
            outNormal = rec.normal;
            refractiveRatio = 1.0 / refIdx;
            cosine = -1.0f * glm::dot(rIn.getDirection(), rec.normal) / glm::length(rIn.getDirection());
        }
        if (refract(rIn.getDirection(), outNormal, refractiveRatio, refracted)){
            reflectProb = schlick(cosine, refIdx);
        }
        else {
            reflectProb = 1.0f;
        }

        std::uniform_real_distribution<> dis(0.0, 1.0);
        if (realRand() < reflectProb){
            scattered = Ray(rec.p, reflected, rIn.getTime());
        }
        else {
            scattered = Ray(rec.p, refracted, rIn.getTime());
        }
        return true;
    }

private:
    float refIdx;
};

class DiffuseLight : public Material {
public:
    DiffuseLight(Texture* a) : emit(a) {}
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered) override { 
        return false;
    }
    virtual glm::vec3 emitted(float u, float v, const glm::vec3& p) const {
        return emit->value(u, v, p);
    }
private:
    Texture* emit;
};

class Isotropic : public Material {
public:
    Isotropic(Texture* tex) : albedo(tex) {}
    virtual bool scatter(Ray& rIn, hitRecord& rec, glm::vec3& attenuation, Ray& scattered) {
        scattered = Ray(rec.p, randomPointInUnitSphere());
        attenuation = albedo->value(rec.u, rec.v, rec.p);
        return true;
    }

private:
    Texture* albedo;
};