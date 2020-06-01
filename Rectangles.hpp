#pragma once
#include "Hitable.hpp"

class Material;

class XYRect : public Hitable {
public:
    XYRect() {}
    XYRect(float x0_in, float x1_in, float y0_in, float y1_in, float z_in, Material* mat) :
    x0(x0_in), x1(x1_in), y0(y0_in), y1(y1_in), z(z_in), matPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        box = AABB(glm::vec3(x0, y0, z - 0.0001), glm::vec3(x1, y1, z + 0.0001));
        return true;
    }

private:
    float x0;
    float x1;
    float y0;
    float y1;
    float z;
    Material* matPtr;
};

bool XYRect::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    float t = (z - ray.getBase().z) / ray.getDirection().z;
    if (t < tMin || t > tMax) {
        return false;
    }
    float x = ray.getBase().x + t * ray.getDirection().x;
    float y = ray.getBase().y + t * ray.getDirection().y;
    if (x < x0 || x > x1 || y < y0 || y > y1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.matPtr = matPtr;
    rec.p = ray.getPointAt(t);
    rec.normal = glm::vec3(0, 0, 1);
    return true;
}

class XZRect : public Hitable {
public:
    XZRect() {}
    XZRect(float x0_in, float x1_in, float z0_in, float z1_in, float y_in, Material* mat) :
    x0(x0_in), x1(x1_in), z0(z0_in), z1(z1_in), y(y_in), matPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        box = AABB(glm::vec3(x0, y - 0.0001, z0), glm::vec3(x1, y + 0.0001, z1));
        return true;
    }

private:
    float x0;
    float x1;
    float z0;
    float z1;
    float y;
    Material* matPtr;
};

bool XZRect::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    float t = (y - ray.getBase().y) / ray.getDirection().y;
    if (t < tMin || t > tMax) {
        return false;
    }
    float x = ray.getBase().x + t * ray.getDirection().x;
    float z = ray.getBase().z + t * ray.getDirection().z;
    if (x < x0 || x > x1 || z < z0 || z > z1) {
        return false;
    }

    rec.u = (x - x0) / (x1 - x0);
    rec.v = (z - z0) / (z1 - z0);
    rec.t = t;
    rec.matPtr = matPtr;
    rec.p = ray.getPointAt(t);
    rec.normal = glm::vec3(0, 1, 0);
    return true;
}

class YZRect : public Hitable {
public:
    YZRect() {}
    YZRect(float y0_in, float y1_in, float z0_in, float z1_in, float x_in, Material* mat) :
    y0(y0_in), y1(y1_in), z0(z0_in), z1(z1_in), x(x_in), matPtr(mat) {}

    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        box = AABB(glm::vec3(x - 0.0001, y0, z0), glm::vec3(x + 0.0001, y1, z1));
        return true;
    }

private:
    float y0;
    float y1;
    float z0;
    float z1;
    float x;
    Material* matPtr;
};

bool YZRect::hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
    float t = (x - ray.getBase().x) / ray.getDirection().x;
    if (t < tMin || t > tMax) {
        return false;
    }
    float y = ray.getBase().y + t * ray.getDirection().y;
    float z = ray.getBase().z + t * ray.getDirection().z;
    if (y < y0 || y > y1 || z < z0 || z > z1) {
        return false;
    }

    rec.u = (z - z0) / (z1 - z0);
    rec.v = (y - y0) / (y1 - y0);
    rec.t = t;
    rec.matPtr = matPtr;
    rec.p = ray.getPointAt(t);
    rec.normal = glm::vec3(1, 0, 0);
    return true;
}

class Box : public Hitable {
public:
    Box(glm::vec3 _p0, glm::vec3 _p1, Material* mat);
    void createSurfaces();
    
    virtual bool hit(const Ray& ray, float tMin, float tMax, hitRecord& rec) const {
        return surfaces->hit(ray, tMin, tMax, rec);
    }
    virtual bool boundingBox(float t0, float t1, AABB& box) const {
        box = AABB(p0, p1);
        return true;
    }

private:
    glm::vec3 p0; // left front down point
    glm::vec3 p1; // right back top point
    HitableList* surfaces;
    Material* matPtr;
};

Box::Box(glm::vec3 _p0, glm::vec3 _p1, Material* mat) {
    p0 = _p0;
    p1 = _p1;
    matPtr = mat;
    createSurfaces();
}

void Box::createSurfaces() {
    Hitable** list = new Hitable*[6];
    
    list[0] = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, matPtr); //top
    list[1] = new InverseNormal(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, matPtr)); //down
    list[2] = new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, matPtr); //left
    list[3] = new InverseNormal(new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, matPtr)); //right
    list[4] = new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, matPtr); //front
    list[5] = new InverseNormal(new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, matPtr)); //back

    surfaces = new HitableList(list, 6);
}