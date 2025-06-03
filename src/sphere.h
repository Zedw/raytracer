#ifndef SPHERE_H
#define SPHERE_H

#include "ray.h"
#include "material.h"

struct Sphere {
    Vec3 center;
    double radius;
    Material material;

    Sphere(const Vec3& c = Vec3(), double r = 1.0, const Material& m = Material())
        : center(c), radius(r), material(m) {}

    bool intersect(const Ray& ray, double& t, Vec3& normal) const {
        Vec3 oc = ray.origin - center;
        double b = dot(oc, ray.direction);
        double c = dot(oc, oc) - radius*radius;
        double h = b*b - c;
        if (h < 0.0) return false;
        h = std::sqrt(h);
        t = -b - h;
        if (t <= 0.0) {
            t = -b + h;
            if (t <= 0.0) return false;
        }
        Vec3 hit = ray.origin + ray.direction*t;
        normal = normalize(hit - center);
        return true;
    }
};

#endif // SPHERE_H
