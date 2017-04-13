#ifndef CURVE_H_
#define CURVE_H_

#include "utils.h"

/** Curve base class
 *  Curve with parameter t in [0,1]
 */
class Curve
{
public:
    virtual ~Curve() {}

    /// Get point on the curve
    virtual Vec2 position(float t) const = 0;

    /// (x:(min, max), y:(min, max)) when t in [x1, x2]
    virtual Box2 xyMinMax(float t1, float t2) const = 0;
};

#endif // CURVE_H_

