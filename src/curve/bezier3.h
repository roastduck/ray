#ifndef BEZIER3_H_
#define BEZIER3_H_

#include <cmath>
#include <cassert>
#include "../curve.h"

/** 3-dgree Bezier Curve
 */
class Bezier3 : public Curve
{
private:
    Vec2 p0, p1, p2, p3;
    Vec2 a, b, c, d; /// Expanded polynomial coefficient
    float tx1, tx2, ty1, ty2; /// Coresponding t for extream values
    Vec2 px1, px2, py1, py2; /// Extream value for x and y

public:
    /// Construct with 4 control points
    Bezier3(const Vec2 &_p0, const Vec2 &_p1, const Vec2 &_p2, const Vec2 &_p3)
        : p0(_p0), p1(_p1), p2(_p2), p3(_p3),
          a(-p0 + 3*p1 - 3*p2 + p3), b(3 * (p0 - 2*p1 + p2)), c(3 * (-p0 + p1)), d(p0)
    {
        Vec2 da(3 * a), db(2 * b), dc(c);
        float deltax(sqrtf(db.x * db.x - 4 * da.x * dc.x)), deltay(sqrtf(db.y * db.y - 4 * da.y * dc.y));
        tx1 = (-b.x - deltax) / (2 * a.x), tx2 = (-b.x + deltax) / (2 * a.x);
        ty1 = (-b.y - deltay) / (2 * a.y), ty2 = (-b.y + deltay) / (2 * a.y);
        px1 = position(tx1), px2 = position(tx2);
        py1 = position(ty1), py2 = position(ty2);
    }

    Vec2 position(float t) const override;
    Vec2 derivation(float t) const override;

    Box2 xyMinMax(float t1, float t2) const override;

private:
    static Vec2 mix(const Vec2 &v1, const Vec2 &v2, float k);
};

inline Vec2 Bezier3::position(float t) const
{
    assert(inrange(t, 0, 1));
    Vec2 _p0(mix(p0, p1, t)), _p1(mix(p1, p2, t)), _p2(mix(p2, p3, t));
    return mix(mix(_p0, _p1, t), mix(_p1, _p2, t), t);
};

inline Vec2 Bezier3::derivation(float t) const
{
    assert(inrange(t, 0, 1));
    Vec2 _p0(p1 - p0), _p1(p2 - p1), _p2(p3 - p2);
    return mix(mix(_p0, _p1, t), mix(_p1, _p2, t), t);
}

inline Box2 Bezier3::xyMinMax(float t1, float t2) const
{
    assert(inrange(t1, 0, 1) && inrange(t2, 0, 1));
    assert(t1 <= t2);
    Box2 ret(position(t1), position(t2));
    if (tx1 >= t1 && tx1 <= t2) ret.enclose(px1);
    if (tx2 >= t1 && tx2 <= t2) ret.enclose(px2);
    if (ty1 >= t1 && ty1 <= t2) ret.enclose(py1);
    if (ty2 >= t1 && ty2 <= t2) ret.enclose(py2);
    return ret;
}

inline Vec2 Bezier3::mix(const Vec2 &v1, const Vec2 &v2, float k)
{
    return Vec2(v1 * (1 - k) + v2 * k);
}

#endif // BEZIER3_H_

