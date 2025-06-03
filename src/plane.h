#ifndef PLANE_H
#define PLANE_H

#include "ray.h"
#include "material.h"

struct Plane {
    Vec3 point;
    Vec3 normal;
    Material material;

    Plane(const Vec3& p = Vec3(), const Vec3& n = Vec3(0,1,0), const Material& m = Material())
        : point(p), normal(normalize(n)), material(m) {}

    bool intersect(const Ray& ray, double& t, Vec3& outNormal) const {
        double denom = dot(normal, ray.direction);
        if (std::fabs(denom) < 1e-6) return false;
        t = dot(point - ray.origin, normal) / denom;
        if (t <= 0.0) return false;
        outNormal = normal;
        return true;
    }
};

#endif // PLANE_H
