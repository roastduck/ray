#ifndef SURFACE_H_
#define SURFACE_H_

#include <memory>
#include <vector>
#include "utils.h"
#include "boxtree.h"
#include "material.h"
#include "photonmap.h"
#include "intersection.h"

/** Surface base class
 *  Surface decided by parameter (u,v) in [0,1] * [0,1]
 */
class Surface
{
private:
    std::unique_ptr<BoxTree> boxTree;

protected:
    virtual Vec3 positionImpl(float u, float v) const = 0;
    virtual Vec3 derivativeUImpl(float u, float v) const = 0;
    virtual Vec3 derivativeVImpl(float u, float v) const = 0;
    virtual Box3 xyzMinMaxImpl(float u1, float u2, float v1, float v2) const = 0;

public:
    Vec3 translate;
    std::unique_ptr<PhotonMap> photonMap;
    std::unique_ptr<Material> material;

    enum Name {
        SYM_BEZIER3 = 1,
        LIGHT_SOURCE = -2,
        INVALID = -1
    };

    Surface()
        : boxTree(nullptr), translate(0, 0, 0), photonMap(nullptr), material(nullptr) {}
    virtual ~Surface() {}

    /// Something can't be put in constructor
    virtual void init();

    /// Get point on the surface
    Vec3 position(float u, float v) const { return positionImpl(u, v) + translate; }
    /// Get partial S / partial u on the surface
    Vec3 derivativeU(float u, float v) const { return derivativeUImpl(u, v); }
    /// Get partial S / partial v on the surface
    Vec3 derivativeV(float u, float v) const { return derivativeVImpl(u, v); }
    // Normal of the surface is defined to be du * dv

    /// (x:(min, max), y:(min, max), z: (min, max)) when (u,v) in [u1, u2] * [v1, v2]
    Box3 xyzMinMax(float u1, float u2, float v1, float v2) const;

    virtual bool uIsCircular() const = 0;
    virtual bool vIsCircular() const = 0;

    virtual bool isLightSource() const { return false; }

    /// Find intersection on 1 surface
    virtual Optional<SurfInterType> findInter(const Ray &ray) const;

    /// Load surfaces from file
    static std::vector< std::unique_ptr<Surface> > load(const char filename[]);
};

inline Box3 Surface::xyzMinMax(float u1, float u2, float v1, float v2) const
{
    Box3 got(xyzMinMaxImpl(u1, u2, v1, v2));
    return Box3(
        got.xMin + translate.x, got.xMax + translate.x,
        got.yMin + translate.y, got.yMax + translate.y,
        got.zMin + translate.z, got.zMax + translate.z
    );
}

#endif // SURFACE_H_

