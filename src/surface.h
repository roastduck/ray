#ifndef SURFACE_H_
#define SURFACE_H_

#include <memory>
#include <vector>
#include "utils.h"
#include "intersection.h"

class BoxTree;

/** Surface base class
 *  Surface decided by parameter (u,v) in [0,1] * [0,1]
 */
class Surface
{
private:
    std::unique_ptr<BoxTree> boxTree;

public:
    enum Name {
        SYM_BEZIER3 = 1,
        INVALID = -1
    };

    virtual ~Surface() {}

    /// Something can't be put in constructor
    void init();

    /// Get point on the surface
    virtual Vec3 position(float u, float v) const = 0;
    /// Get partial S / partial u on the surface
    virtual Vec3 derivativeU(float u, float v) const = 0;
    /// Get partial S / partial v on the surface
    virtual Vec3 derivativeV(float u, float v) const = 0;

    /// (x:(min, max), y:(min, max), z: (min, max)) when (u,v) in [u1, u2] * [v1, v2]
    virtual Box3 xyzMinMax(float u1, float u2, float v1, float v2) const = 0;

    virtual bool uIsCircular() = 0;
    virtual bool vIsCircular() = 0;

    Optional<InterType> findInter(const Ray &ray) const;

    /// Load surfaces from file
    static std::vector< std::unique_ptr<Surface> > load(const char filename[]);
};

#endif // SURFACE_H_

