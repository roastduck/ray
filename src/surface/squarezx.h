#ifndef SQUAREZX_H_
#define SQUAREZX_H_

#include "../utils.h"
#include "../const.h"
#include "../surface.h"
#include "../intersection.h"

/** Square on z-x plane
 *  y+ means outwards
 */
class SquareZX : public Surface
{
private:
    float edgeX, edgeZ;

protected:
    void init() override {} /// Do nothing because findInter is overrided
    Vec3 positionImpl(float u, float v) const override { return Vec3(v * edgeX, 0, u * edgeZ); }
    Vec3 derivativeUImpl(float u, float v) const override { return Vec3(0, 0, 1); }
    Vec3 derivativeVImpl(float u, float v) const override { return Vec3(1, 0, 0); }

    Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const override
    {
        return Box3(v1 * edgeX, v2 * edgeX, 0, 0, u1 * edgeZ, u2 * edgeZ);
    }

public:
    SquareZX(float _edgeX, float _edgeZ) : edgeX(_edgeX), edgeZ(_edgeZ) {}

    bool uIsCircular() const override { return false; }
    bool vIsCircular() const override { return false; }

    Optional<SurfInterType> findInter(const Ray &ray) const override
    {
        auto interOpt = intersecXZ(translate.y, Box2(translate.x, edgeX + translate.x, translate.z, edgeZ + translate.z), ray);
        if (!interOpt.isOk()) return None();
        auto inter(interOpt.ok());
        if (inter.second < EPS) return None();
        float u = (inter.first.z - translate.z) / edgeZ;
        float v = (inter.first.x - translate.x) / edgeX;
        return SurfInterType(this, inter.second, u, v, inter.first, Vec3(0, 1, 0));
    }
};

#endif // SQUAREZX_H_

