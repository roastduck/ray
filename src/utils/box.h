#ifndef BOX_H_
#define BOX_H_

#include <cassert>
#include <algorithm>
#include "vec.h"

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
    Box3(const Vec3 &v) : Box3(v.x, v.x, v.y, v.y, v.z, v.z) {}

    float size() const
    {
        return (xMax - xMin) * (yMax - yMin) * (zMax - zMin);
    }

    bool contains(const Vec3 &v) const
    {
        return v.x >= xMin && v.x <= xMax && v.y >= yMin && v.y <= yMax && v.z >= zMin && v.z <= zMax;
    }

    float dist2(const Vec3 &v) const
    {
        float dx(std::max(0.0f, std::max(v.x - xMax, xMin - v.x)));
        float dy(std::max(0.0f, std::max(v.y - yMax, yMin - v.y)));
        float dz(std::max(0.0f, std::max(v.z - zMax, zMin - v.z)));
        return Vec3(dx, dy, dz).dist2();
    }

    Box2 xySide() const { return Box2(xMin, xMax, yMin, yMax); }
    Box2 yzSide() const { return Box2(yMin, yMax, zMin, zMax); }
    Box2 xzSide() const { return Box2(xMin, xMax, zMin, zMax); }
};

inline Box3 merge(const Box3 &lhs, const Box3 &rhs)
{
    return Box3(
        std::min(lhs.xMin, rhs.xMin),
        std::max(lhs.xMax, rhs.xMax),
        std::min(lhs.yMin, rhs.yMin),
        std::max(lhs.yMax, rhs.yMax),
        std::min(lhs.zMin, rhs.zMin),
        std::max(lhs.zMax, rhs.zMax)
    );
}

#endif // BOX_H_

