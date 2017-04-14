#include <cassert>
#include "intersection.h"

Optional<InterType> intersecYZ(float x, const Box2 &box, const Ray &ray)
{
    float t((x - ray.st.x) / ray.dir.x);
    assert(t >= 0);
    Vec3 p(ray.st + t * ray.dir);
    if (box.contains(Vec2(p.y, p.z)))
        return InterType(p, t);
    else
        return None();
}

Optional<InterType> intersecXZ(float y, const Box2 &box, const Ray &ray)
{
    float t((y - ray.st.y) / ray.dir.y);
    assert(t >= 0);
    Vec3 p(ray.st + t * ray.dir);
    if (box.contains(Vec2(p.x, p.z)))
        return InterType(p, t);
    else
        return None();
}

Optional<InterType> intersecXY(float z, const Box2 &box, const Ray &ray)
{
    float t((z - ray.st.z) / ray.dir.z);
    assert(t >= 0);
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

