#ifndef SQUAREXY_H_
#define SQUAREXY_H_

#include "../utils.h"
#include "../const.h"
#include "../surface.h"
#include "../intersection.h"

/** Square on x-y plane
 *  Upwards means outwards
 */
class SquareXY : public Surface
{
private:
    float edgeX, edgeY;

protected:
    void init() override {} /// Do nothing because findInter is overrided
    Vec3 positionImpl(float u, float v) const override { return Vec3(u * edgeX, v * edgeY, 0); }
    Vec3 derivativeUImpl(float u, float v) const override { return Vec3(1, 0, 0); }
    Vec3 derivativeVImpl(float u, float v) const override { return Vec3(0, 1, 0); }

    Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const override
    {
        return Box3(u1 * edgeX, u2 * edgeX, v1 * edgeY, v2 * edgeY, 0, 0);
    }

public:
    SquareXY(float _edgeX, float _edgeY) : edgeX(_edgeX), edgeY(_edgeY) {}

    bool uIsCircular() const override { return false; }
    bool vIsCircular() const override { return false; }

    Optional<SurfInterType> findInter(const Ray &ray) const override
    {
        auto interOpt = intersecXY(translate.z, Box2(translate.x, edgeX + translate.x, translate.y, edgeY + translate.y), ray);
        if (!interOpt.isOk()) return None();
        auto inter(interOpt.ok());
        if (inter.second < EPS) return None();
        float u = (inter.first.x - translate.x) / edgeX;
        float v = (inter.first.y - translate.y) / edgeY;
        return SurfInterType(this, inter.second, u, v, inter.first, Vec3(0, 0, 1));
    }
};

#endif // SQUAREXY_H_

