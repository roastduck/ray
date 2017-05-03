#ifndef AXISYMMETRIC_H_
#define AXISYMMETRIC_H_

#include <cassert>
#include <memory>
#include "../surface.h"
#include "../curve.h"

/** Rotate a curve around z-axis to form a surface
 *  omega = u * 2 * PI, curve param = v
 *  Curve(x, y) => Surface(z, x ~ y)
 *  If curve parameter v is positively correlated with x, normal goes outwards
 */
class Axisymmetric : public Surface
{
private:
    std::unique_ptr<const Curve> curve; /// Responsible to destroy it

protected:
    Vec3 positionImpl(float u, float v) const override;
    Vec3 derivativeUImpl(float u, float v) const override;
    Vec3 derivativeVImpl(float u, float v) const override;

    Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const override;

public:
    Axisymmetric(const Curve *_curve) : curve(_curve) {}
    Axisymmetric(const Axisymmetric &) = delete;
    Axisymmetric &operator=(const Axisymmetric &) = delete;

    bool uIsCircular() const override { return true; }
    bool vIsCircular() const override { return false; }
};

inline Vec3 Axisymmetric::positionImpl(float u, float v) const
{
    Vec2 flat = curve->position(v);
    float omega = u * 2 * PI;
    return Vec3(flat.y * cosf(omega), flat.y * sinf(omega), flat.x);
}

inline Vec3 Axisymmetric::derivativeUImpl(float u, float v) const
{
    Vec2 flat = curve->position(v);
    float omega = u * 2 * PI;
    return Vec3(flat.y * 2 * PI * -sinf(omega), flat.y * 2 * PI * cosf(omega), 0);
}

inline Vec3 Axisymmetric::derivativeVImpl(float u, float v) const
{
    Vec2 dflat = curve->derivation(v);
    float omega = u * 2 * PI;
    return Vec3(dflat.y * cosf(omega), dflat.y * sinf(omega), dflat.x);
}

inline Box3 Axisymmetric::xyzMinMaxImpl(float u1, float u2, float v1, float v2) const
{
    assert(inrange(u1, 0, 1) && inrange(u2, 0, 1) && inrange(v1, 0, 1) && inrange(v2, 0, 1));
    assert(v1 <= v2);
    Box2 curvBox(curve->xyMinMax(v1, v2));
    float cos1(cos(u1 * 2 * PI)), sin1(sin(u1 * 2 * PI)), cos2(cos(u2 * 2 * PI)), sin2(sin(u2 * 2 * PI));
    Vec2 p00(curvBox.yMin * cos1, curvBox.yMin * sin1), p01(curvBox.yMax * cos1, curvBox.yMax * sin1);
    Vec2 p10(curvBox.yMin * cos2, curvBox.yMin * sin2), p11(curvBox.yMax * cos2, curvBox.yMax * sin2);
    return Box3(
        u1 <= 0.5 && u2 >= 0.5 ? -curvBox.yMax : std::min(std::min(p00.x, p01.x), std::min(p10.x, p11.x)),
        u1 > u2 ? curvBox.yMax : std::max(std::max(p00.x, p01.x), std::max(p10.x, p11.x)),
        u1 <= 0.75 && u2 >= 0.75 ? -curvBox.yMax : std::min(std::min(p00.y, p01.y), std::min(p10.y, p11.y)),
        u1 <= 0.25 && u2 >= 0.25 ? curvBox.yMax : std::max(std::max(p00.y, p01.y), std::max(p10.y, p11.y)),
        curvBox.xMin,
        curvBox.xMax
    );
}

#endif // AXISYMMETRIC_H_

