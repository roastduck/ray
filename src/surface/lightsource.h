#ifndef LIGHT_SOURCE_H_
#define LIGHT_SOURCE_H_

#include <cmath>
#include <cassert>
#include "../const.h"
#include "../utils.h"
#include "../surface.h"
#include "../intersection.h"

/** Light source as a ball
 *  Cannot be used as a normal object
 */
class LightSource : public Surface
{
public:
    float radius;
    color_t color;
    Vec3 direction;

    LightSource(float _radius, const color_t &_color, const Vec3 &_direction)
        : radius(_radius), color(_color), direction(_direction) {}

protected:
    void init() override {} /// Do nothing because findInter is overrided
    Vec3 positionImpl(float u, float v) const override { assert(false); } /// Forbidden
    Vec3 derivativeUImpl(float u, float v) const override { assert(false); }
    Vec3 derivativeVImpl(float u, float v) const override { assert(false); }
    Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const override { assert(false); }

public:
    bool uIsCircular() const override { assert(false); }
    bool vIsCircular() const override { assert(false); }
    bool isLightSource() const override { return true; }

    /// This function is can only returns t and pos
    Optional<SurfInterType> findInter(const Ray &ray) const override;
};

inline Optional<SurfInterType> LightSource::findInter(const Ray &ray) const
{
    Vec3 _st(ray.st - translate);
    double a(ray.dir.dist2()), b(2 * dot(_st, ray.dir)), c(_st.dist2() - sqr(radius));
    double delta = b * b - 4 * a * c;
    if (delta < 0) return None();
    delta = sqrt(delta);
    float t = (-b - delta) / (2 * a);
    if (t < EPS)
        t = (-b + delta) / (2 * a);
    if (t < EPS)
        return None();
    return SurfInterType(this, t, ray.st + t * ray.dir, Vec3(0, 0, 0));
}

#endif // LIGHT_SOURCE_H_

