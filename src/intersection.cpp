#include <cassert>
#include "const.h"
#include "surface.h"
#include "intersection.h"

Optional<InterType> intersecYZ(float x, const Box2 &box, const Ray &ray)
{
    float t((x - ray.st.x) / ray.dir.x);
    if (!std::isfinite(t)) return None();
    Vec3 p(ray.st + t * ray.dir);
    if (box.contains(Vec2(p.y, p.z)))
        return InterType(p, t);
    else
        return None();
}

Optional<InterType> intersecXZ(float y, const Box2 &box, const Ray &ray)
{
    float t((y - ray.st.y) / ray.dir.y);
    if (!std::isfinite(t)) return None();
    Vec3 p(ray.st + t * ray.dir);
    if (box.contains(Vec2(p.x, p.z)))
        return InterType(p, t);
    else
        return None();
}

Optional<InterType> intersecXY(float z, const Box2 &box, const Ray &ray)
{
    float t((z - ray.st.z) / ray.dir.z);
    if (!std::isfinite(t)) return None();
    Vec3 p(ray.st + t * ray.dir);
    if (box.contains(Vec2(p.x, p.y)))
        return InterType(p, t);
    else
        return None();
}

Optional<InterType> intersec(const Box3 &box, const Ray &ray)
{
    if (box.contains(ray.st))
        return InterType(ray.st, 0);

    if (ray.st.x <= box.xMin && ray.dir.x <= 0) return None();
    if (ray.st.x >= box.xMax && ray.dir.x >= 0) return None();
    if (ray.st.y <= box.yMin && ray.dir.y <= 0) return None();
    if (ray.st.y >= box.yMax && ray.dir.y >= 0) return None();
    if (ray.st.z <= box.zMin && ray.dir.z <= 0) return None();
    if (ray.st.z >= box.zMax && ray.dir.z >= 0) return None();

    Optional<InterType> ret;
    if (ray.st.x <= box.xMin && (ret = intersecYZ(box.xMin, box.yzSide(), ray)).isOk()) return ret;
    if (ray.st.x >= box.xMax && (ret = intersecYZ(box.xMax, box.yzSide(), ray)).isOk()) return ret;
    if (ray.st.y <= box.yMin && (ret = intersecXZ(box.yMin, box.xzSide(), ray)).isOk()) return ret;
    if (ray.st.y >= box.yMax && (ret = intersecXZ(box.yMax, box.xzSide(), ray)).isOk()) return ret;
    if (ray.st.z <= box.zMin && (ret = intersecXY(box.zMin, box.xySide(), ray)).isOk()) return ret;
    if (ray.st.z >= box.zMax && (ret = intersecXY(box.zMax, box.xySide(), ray)).isOk()) return ret;
    return None();
}

Optional<SurfInterType> intersec(float t, float u, float v, const Vec3 &p0, const Surface &surf, const Ray &ray)
{
    Vec3 s(surf.position(u, v)), c(p0), f(c - s);
    Vec3 su(surf.derivativeU(u, v)), sv(surf.derivativeV(u, v));
    const Vec3 &ct(ray.dir);

    for (int i = 0; i < 5; i++)
    {
        u -= dot(ct, cross(sv, f)) / dot(ct, cross(sv, -su)); // 1
        if (!std::isfinite(u)) return None();
        s = surf.position(u, v), f = c - s, su = surf.derivativeU(u, v), sv = surf.derivativeV(u, v);

        v -= dot(ct, cross(su, f)) / dot(ct, cross(su, -sv)); // 2
        if (!std::isfinite(v)) return None();
        s = surf.position(u, v), f = c - s, su = surf.derivativeU(u, v), sv = surf.derivativeV(u, v);

        t -= dot(su, cross(sv, f)) / dot(su, cross(sv, ct)); // 3
        if (!std::isfinite(t)) return None();
        c = ray.st + t * ray.dir, f = c - s;

        if (f.dist2() < EPS * EPS)
        {
            if (t > EPS /*Move out of original position*/ && inrange(u, 0, 1) && inrange(v, 0, 1))
                return SurfInterType(&surf, t, u, v, s, cross(su, sv));
            else
                return None();
        }
    }
    return None();
}

