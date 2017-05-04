#ifndef SQUAREYZ_H_
#define SQUAREYZ_H_

#include "../utils.h"
#include "../const.h"
#include "../surface.h"
#include "../intersection.h"

/** Square on y-z plane
 *  x+ means outwards
 */
class SquareYZ : public Surface
{
private:
    float edge;

protected:
    void init() override {} /// Do nothing because findInter is overrided
    Vec3 positionImpl(float u, float v) const override { return Vec3(0, u * edge, v * edge); }
    Vec3 derivativeUImpl(float u, float v) const override { return Vec3(0, 1, 0); }
    Vec3 derivativeVImpl(float u, float v) const override { return Vec3(0, 0, 1); }

    Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const override
    {
        return Box3(0, 0, u1 * edge, u2 * edge, v1 * edge, v2 * edge);
    }

public:
    SquareYZ(float _edge) : edge(_edge) {}

    bool uIsCircular() const override { return false; }
    bool vIsCircular() const override { return false; }

    Optional<SurfInterType> findInter(const Ray &ray) const override
    {
        auto interOpt = intersecYZ(translate.x, Box2(translate.y, edge + translate.y, translate.z, edge + translate.z), ray);
        if (!interOpt.isOk()) return None();
        auto inter(interOpt.ok());
        if (inter.second < EPS) return None();
        return SurfInterType(this, inter.second, inter.first, Vec3(1, 0, 0));
    }
};

#endif // SQUAREYZ_H_

