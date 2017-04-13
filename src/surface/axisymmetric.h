#ifndef AXISYMMETRIC_H_
#define AXISYMMETRIC_H_

#include <memory>
#include "../surface.h"
#include "../curve.h"

/** Rotate a curve around z-axis to form a surface
 *  omega = u * 2 * PI, curve param = v
 *  Curve(x, y) => Surface(z, x ~ y)
 */
class Axisymmetric : public Surface
{
private:
    std::unique_ptr<const Curve> curve; /// Responsible to destroy it

public:
    Axisymmetric(const Curve *_curve) : curve(_curve) {}
    Axisymmetric(const Axisymmetric &) = delete;
    Axisymmetric &operator=(const Axisymmetric &) = delete;

    Vec3 position(float u, float v) const override;

    bool uIsCircular() override { return true; }
    bool vIsCircular() override { return false; }
};

inline Vec3 Axisymmetric::position(float u, float v) const
{
    Vec2 flat = curve->position(v);
    float omega = u * 2 * PI;
    return Vec3(flat.y * cosf(omega), flat.y * sinf(omega), flat.x);
}

#endif // AXISYMMETRIC_H_

