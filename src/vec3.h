#ifndef VEC3_H
#define VEC3_H

#include <cmath>

struct Vec3 {
    double x, y, z;
    Vec3(double xx=0, double yy=0, double zz=0) : x(xx), y(yy), z(zz) {}
    Vec3 operator+(const Vec3& v) const { return Vec3(x+v.x, y+v.y, z+v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x-v.x, y-v.y, z-v.z); }
    Vec3 operator*(double s) const { return Vec3(x*s, y*s, z*s); }
    Vec3 operator*(const Vec3& v) const { return Vec3(x*v.x, y*v.y, z*v.z); }
    Vec3 operator/(double s) const { return Vec3(x/s, y/s, z/s); }
};

inline Vec3 normalize(const Vec3& v) {
    double len = std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
    return v / len;
}

inline double dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - n * (2.0 * dot(v, n));
}

inline Vec3 cross(const Vec3& a, const Vec3& b) {
    return Vec3(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

#endif // VEC3_H
