#ifndef RAY_H
#define RAY_H

#include "vec3.h"

struct Ray {
    Vec3 origin;
    Vec3 direction;
    Ray(const Vec3& o = Vec3(), const Vec3& d = Vec3()) : origin(o), direction(normalize(d)) {}
};

#endif // RAY_H
