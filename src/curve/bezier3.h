#ifndef BEZIER3_H_
#define BEZIER3_H_

#include "../curve.h"

/** 3-dgree Bezier Curve
 */
class Bezier3 : public Curve
{
private:
    Vec2 p0, p1, p2, p3;

public:
    /// Construct with 4 control points
    Bezier3(const Vec2 &_p0, const Vec2 &_p1, const Vec2 &_p2, const Vec2 &_p3)
        : p0(_p0), p1(_p1), p2(_p2), p3(_p3) {}

    Vec2 position(float x) const override;

private:
    static Vec2 mix(const Vec2 &v1, const Vec2 &v2, float k);
};

inline Vec2 Bezier3::position(float x) const
{
    Vec2 _p0(mix(p0, p1, x)), _p1(mix(p1, p2, x)), _p2(mix(p2, p3, x));
    return mix(mix(_p0, _p1, x), mix(_p1, _p2, x), x);
};

inline Vec2 Bezier3::mix(const Vec2 &v1, const Vec2 &v2, float k)
{
    return Vec2(v1 * k + v2 * (1 - k));
}

#endif // BEZIER3_H_

