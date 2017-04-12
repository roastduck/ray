#ifndef UTILS_H_
#define UTILS_H_

#include <cmath>
#include <iomanip>
#include <iostream>

struct Vec2
{
    float x, y;
    Vec2() {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}
};

struct Vec3
{
    float x, y, z;
    Vec3() {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

inline std::istream &operator>>(std::istream &is, Vec2 &v)
{
    is >> v.x >> v.y; return is;
}
inline std::istream &operator>>(std::istream &is, Vec3 &v)
{
    is >> v.x >> v.y >> v.z; return is;
}
inline std::ostream &operator<<(std::ostream &os, const Vec2 &v)
{
    os << std::fixed << std::setprecision(3) << v.x << " " << v.y; return os;
}
inline std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    os << std::fixed << std::setprecision(3) << v.x << " " << v.y << " " << v.z; return os;
}

inline Vec2 operator+(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs.x + rhs.x, lhs.y + rhs.y); }
inline Vec2 operator*(const Vec2 &v, float k) { return Vec2(v.x * k, v.y * k); }
inline Vec2 operator*(float k, const Vec2 &v) { return Vec2(v.x * k, v.y * k); }

inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline Vec3 operator*(const Vec3 &v, float k) { return Vec3(v.x * k, v.y * k, v.z * k); }
inline Vec3 operator*(float k, const Vec3 &v) { return Vec3(v.x * k, v.y * k, v.z * k); }

const float PI = acosf(-1);

#endif // UTILS_H_

