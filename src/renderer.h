#ifndef RENDERER_H
#define RENDERER_H

#include "sphere.h"
#include "plane.h"

struct Scene {
    Sphere sphere;
    Plane plane;
};

inline Vec3 trace(const Ray& ray, const Scene& scene, int depth = 2) {
    if (depth == 0) return Vec3(0.2, 0.7, 1.0); // sky color

    double tSphere, tPlane;
    Vec3 nSphere, nPlane;
    bool hitSphere = scene.sphere.intersect(ray, tSphere, nSphere);
    bool hitPlane = scene.plane.intersect(ray, tPlane, nPlane);

    double t = std::numeric_limits<double>::max();
    Vec3 normal;
    Material material;
    if (hitSphere && tSphere < t) {
        t = tSphere;
        normal = nSphere;
        material = scene.sphere.material;
    }
    if (hitPlane && tPlane < t) {
        t = tPlane;
        normal = nPlane;
        material = scene.plane.material;
    }

    if (t == std::numeric_limits<double>::max()) {
        return Vec3(0.2, 0.7, 1.0); // sky
    }

    Vec3 hitPoint = ray.origin + ray.direction * t;
    Vec3 reflectedDir = reflect(ray.direction, normal);
    Ray reflectedRay(hitPoint + reflectedDir * 1e-4, reflectedDir);
    Vec3 reflectedColor = trace(reflectedRay, scene, depth - 1);
    return material.color * (1.0 - material.reflectivity) + reflectedColor * material.reflectivity;
}

#endif // RENDERER_H
