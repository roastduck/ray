#ifndef UTILS_H_
#define UTILS_H_

#include <cmath>
#include <iomanip>
#include <iostream>
#include <algorithm>

struct Vec2
{
    float x, y;
    Vec2() {}
    Vec2(float _x, float _y) : x(_x), y(_y) {}

    Vec2 operator-() const { return Vec2(-x, -y); }
};

struct Vec3
{
    float x, y, z;
    Vec3() {}
    Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

    Vec3 operator-() const { return Vec3(-x, -y, -z); }
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
inline Vec2 operator-(const Vec2 &lhs, const Vec2 &rhs) { return Vec2(lhs.x - rhs.x, lhs.y - rhs.y); }
inline Vec2 operator*(const Vec2 &v, float k) { return Vec2(v.x * k, v.y * k); }
inline Vec2 operator*(float k, const Vec2 &v) { return Vec2(v.x * k, v.y * k); }

inline Vec3 operator+(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
inline Vec3 operator-(const Vec3 &lhs, const Vec3 &rhs) { return Vec3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
inline Vec3 operator*(const Vec3 &v, float k) { return Vec3(v.x * k, v.y * k, v.z * k); }
inline Vec3 operator*(float k, const Vec3 &v) { return Vec3(v.x * k, v.y * k, v.z * k); }

struct Box2
{
    float xMin, xMax, yMin, yMax;

    Box2(Vec2 p1, Vec2 p2)
        : xMin(std::min(p1.x, p2.x)), xMax(std::max(p1.x, p2.x)),
          yMin(std::min(p1.y, p2.y)), yMax(std::max(p1.y, p2.y))
    {}

    void enclose(Vec2 p)
    {
        xMin = std::min(xMin, p.x), xMax = std::max(xMax, p.x);
        yMin = std::min(yMin, p.y), yMax = std::max(yMax, p.y);
    }
};

struct Box3
{
    float xMin, xMax, yMin, yMax, zMin, zMax;

    Box3(float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax)
        : xMin(_xMin), xMax(_xMax), yMin(_yMin), yMax(_yMax), zMin(_zMin), zMax(_zMax) {}
};

inline bool inrange(float x, float l, float r)
{
    return x >= l && x <= r;
}

const float PI = acosf(-1);

#endif // UTILS_H_

