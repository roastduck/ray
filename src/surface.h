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

public:
    PhotonMap photonMap;
    Material material;

    enum Name {
        SYM_BEZIER3 = 1,
        INVALID = -1
    };

    Surface(const Material &_material)
        : boxTree(nullptr), photonMap(*this), material(_material) {}
    virtual ~Surface() {}

    /// Something can't be put in constructor
    void init();

    /// Get point on the surface
    virtual Vec3 position(float u, float v) const = 0;
    /// Get partial S / partial u on the surface
    virtual Vec3 derivativeU(float u, float v) const = 0;
    /// Get partial S / partial v on the surface
    virtual Vec3 derivativeV(float u, float v) const = 0;
    // Normal of the surface is defined to be du * dv

    /// (x:(min, max), y:(min, max), z: (min, max)) when (u,v) in [u1, u2] * [v1, v2]
    virtual Box3 xyzMinMax(float u1, float u2, float v1, float v2) const = 0;

    virtual bool uIsCircular() = 0;
    virtual bool vIsCircular() = 0;

    struct SurfInterType
    {
        const Surface *surf;
        float t, u, v;
        Vec3 pos, normal;
        SurfInterType() : surf(0) {}
        SurfInterType(const Surface *_surf, float _t, float _u, float _v, const Vec3 &_pos, const Vec3 &_normal)
            : surf(_surf), t(_t), u(_u), v(_v), pos(_pos), normal(_normal) {}
    };

    /// Find intersection on 1 surface
    Optional<SurfInterType> findInter(const Ray &ray) const;

    /// Load surfaces from file
    static std::vector< std::unique_ptr<Surface> > load(const char filename[]);
};

#endif // SURFACE_H_

