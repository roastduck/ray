#ifndef VEC_H_
#define VEC_H_

#include <cassert>
#include <iomanip>
#include <iostream>

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

#endif // VEC_H_

