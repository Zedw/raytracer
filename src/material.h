#ifndef MATERIAL_H
#define MATERIAL_H

#include "vec3.h"

struct Material {
    Vec3 color;
    double reflectivity; // 0 = matte, 1 = perfect mirror
    Material(const Vec3& c = Vec3(1,1,1), double r = 0.0) : color(c), reflectivity(r) {}
};

#endif // MATERIAL_H
