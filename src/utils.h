/** Miscellaneous
 *  Vectors, Boxes, helper functions, mathematical constants
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <cmath>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <algorithm>

template <class T>
struct Vec2t
{
    T x, y;
    Vec2t() {}
    Vec2t(T _x, T _y) : x(_x), y(_y) {}
    template <class T2> Vec2t(const Vec2t<T2> &rhs) : x(rhs.x), y(rhs.y) {}

    Vec2t operator-() const { return Vec2t(-x, -y); }
    T dist2() const { return x * x + y * y; }
};

template <class T>
struct Vec3t
{
    T x, y, z;
    Vec3t() {}
    Vec3t(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
    template <class T2> Vec3t(const Vec3t<T2> &rhs) : x(rhs.x), y(rhs.y), z(rhs.z) {}

    Vec3t operator-() const { return Vec3t(-x, -y, -z); }
    T dist2() const { return x * x + y * y + z * z; }
};

template <class T>
inline std::istream &operator>>(std::istream &is, Vec2t<T> &v)
{
    is >> v.x >> v.y; return is;
}
template <class T>
inline std::istream &operator>>(std::istream &is, Vec3t<T> &v)
{
    is >> v.x >> v.y >> v.z; return is;
}
template <class T>
inline std::ostream &operator<<(std::ostream &os, const Vec2t<T> &v)
{
    os << std::fixed << std::setprecision(3) << v.x << " " << v.y; return os;
}
template <class T>
inline std::ostream &operator<<(std::ostream &os, const Vec3t<T> &v)
{
    os << std::fixed << std::setprecision(3) << v.x << " " << v.y << " " << v.z; return os;
}

template <class T>
inline Vec2t<T> operator+(const Vec2t<T> &lhs, const Vec2t<T> &rhs) { return Vec2t<T>(lhs.x + rhs.x, lhs.y + rhs.y); }
template <class T>
inline Vec2t<T> operator-(const Vec2t<T> &lhs, const Vec2t<T> &rhs) { return Vec2t<T>(lhs.x - rhs.x, lhs.y - rhs.y); }
template <class T>
inline Vec2t<T> operator*(const Vec2t<T> &v, T k) { return Vec2t<T>(v.x * k, v.y * k); }
template <class T>
inline Vec2t<T> operator*(T k, const Vec2t<T> &v) { return Vec2t<T>(v.x * k, v.y * k); }

template <class T>
inline Vec3t<T> operator+(const Vec3t<T> &lhs, const Vec3t<T> &rhs) { return Vec3t<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
template <class T>
inline Vec3t<T> operator-(const Vec3t<T> &lhs, const Vec3t<T> &rhs) { return Vec3t<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
template <class T>
inline Vec3t<T> operator*(const Vec3t<T> &v, T k) { return Vec3t<T>(v.x * k, v.y * k, v.z * k); }
template <class T>
inline Vec3t<T> operator*(T k, const Vec3t<T> &v) { return Vec3t<T>(v.x * k, v.y * k, v.z * k); }

template <class T>
inline Vec3t<T> cross(const Vec3t<T> &lhs, const Vec3t<T> &rhs)
{
    return Vec3t<T>(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);
}

template <class T>
inline T dot(const Vec3t<T> &lhs, const Vec3t<T> &rhs)
{
    return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

typedef Vec2t<float> Vec2; /// Other type only used in few situations to increase accuracy
typedef Vec3t<float> Vec3;

struct Box2
{
    float xMin, xMax, yMin, yMax;

    Box2(float _xMin, float _xMax, float _yMin, float _yMax)
        : xMin(_xMin), xMax(_xMax), yMin(_yMin), yMax(_yMax) {}
    Box2(Vec2 p1, Vec2 p2)
        : xMin(std::min(p1.x, p2.x)), xMax(std::max(p1.x, p2.x)),
          yMin(std::min(p1.y, p2.y)), yMax(std::max(p1.y, p2.y))
    {}

    void enclose(Vec2 p)
    {
        xMin = std::min(xMin, p.x), xMax = std::max(xMax, p.x);
        yMin = std::min(yMin, p.y), yMax = std::max(yMax, p.y);
    }

    bool contains(const Vec2 &v) const
    {
        return v.x >= xMin && v.x <= xMax && v.y >= yMin && v.y <= yMax;
    }
};

struct Box3
{
    float xMin, xMax, yMin, yMax, zMin, zMax;

    Box3() {}
    Box3(float _xMin, float _xMax, float _yMin, float _yMax, float _zMin, float _zMax)
        : xMin(_xMin), xMax(_xMax), yMin(_yMin), yMax(_yMax), zMin(_zMin), zMax(_zMax)
    {
        assert(xMin <= xMax && yMin <= yMax && zMin <= zMax);
        assert(xMin < xMax || yMin < yMax || zMin < zMax);
    }

    float size() const
    {
        return (xMax - xMin) * (yMax - yMin) * (zMax - zMin);
    }

    bool contains(const Vec3 &v) const
    {
        return v.x >= xMin && v.x <= xMax && v.y >= yMin && v.y <= yMax && v.z >= zMin && v.z <= zMax;
    }

    Box2 xySide() const { return Box2(xMin, xMax, yMin, yMax); }
    Box2 yzSide() const { return Box2(yMin, yMax, zMin, zMax); }
    Box2 xzSide() const { return Box2(xMin, xMax, zMin, zMax); }
};

struct Ray
{
    Vec3 st; /// Starting point
    Vec3 dir; /// Direction

    Ray(const Vec3 &_st, const Vec3 &_dir)
        : st(_st), dir(_dir) {}
};

class None {};

template <class T>
class Optional
{
private:
    bool valid;
    T content;

public:
    Optional() : valid(false) {}
    Optional(None) : valid(false) {}
    Optional(const T &_content) : valid(true), content(_content) {}
    Optional(T &&_content) : valid(true), content(_content) {}

    bool isOk() const { return valid; }
    T &ok() { assert(valid); return content; }
    const T &ok() const { assert(valid); return content; }
};

inline bool inrange(float x, float l, float r)
{
    return x >= l && x <= r;
}

const float PI = acosf(-1);

#endif // UTILS_H_

