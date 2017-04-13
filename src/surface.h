#ifndef SURFACE_H_
#define SURFACE_H_

#include <memory>
#include <vector>
#include "utils.h"

/** Surface base class
 *  Surface decided by parameter (u,v) in [0,1] * [0,1]
 */
class Surface
{
public:
    enum Name {
        SYM_BEZIER3 = 1,
        INVALID = -1
    };

    virtual ~Surface() {}

    /// Get point on the surface
    virtual Vec3 position(float u, float v) const = 0;

    /// (x:(min, max), y:(min, max), z: (min, max)) when (u,v) in [u1, u2] * [v1, v2]
    virtual Box3 xyzMinMax(float u1, float u2, float v1, float v2) const = 0;

    virtual bool uIsCircular() = 0;
    virtual bool vIsCircular() = 0;

    /// Load surfaces from file
    static std::vector< std::unique_ptr<Surface> > load(const char filename[]);
};

#endif // SURFACE_H_

