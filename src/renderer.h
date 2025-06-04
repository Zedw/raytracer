#ifndef RENDERER_H
#define RENDERER_H

#include "sphere.h"
#include "plane.h"
#include <cmath>

struct Scene {
    Sphere sphere;
    Plane plane;
    Vec3 lightDir;
    Vec3 lightColor;
};

inline bool occluded(const Ray& shadowRay, const Scene& scene) {
    double tS, tP; Vec3 n;
    if (scene.sphere.intersect(shadowRay, tS, n)) return true;
    if (scene.plane.intersect(shadowRay, tP, n)) return true;
    return false;
}

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

    // direct lighting with soft shadows
    Vec3 L = scene.lightDir;
    Vec3 Lperp1 = normalize(cross(L, Vec3(0,1,0)));
    if (std::fabs(Lperp1.x) < 1e-6 && std::fabs(Lperp1.y) < 1e-6 && std::fabs(Lperp1.z) < 1e-6)
        Lperp1 = Vec3(1,0,0);
    Vec3 Lperp2 = normalize(cross(L, Lperp1));
    const double radius = 0.1;
    Vec3 dirs[5] = {
        L,
        normalize(L + Lperp1 * radius),
        normalize(L - Lperp1 * radius),
        normalize(L + Lperp2 * radius),
        normalize(L - Lperp2 * radius)
    };

    double lightSum = 0.0;
    for (Vec3 d : dirs) {
        Ray shadowRay(hitPoint + normal * 1e-4, d);
        if (!occluded(shadowRay, scene)) {
            lightSum += std::max(0.0, dot(normal, d));
        }
    }
    double diffuse = lightSum / 5.0;

    Vec3 local = material.color * (0.1 + diffuse) * scene.lightColor;

    Vec3 reflectedDir = reflect(ray.direction, normal);
    Ray reflectedRay(hitPoint + reflectedDir * 1e-4, reflectedDir);
    Vec3 reflectedColor = trace(reflectedRay, scene, depth - 1);

    return local * (1.0 - material.reflectivity) + reflectedColor * material.reflectivity;
}

#endif // RENDERER_H
