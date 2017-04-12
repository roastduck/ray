#ifndef CURVE_H_
#define CURVE_H_

#include "utils.h"

/** Curve base class
 *  Curve with parameter x in [0,1]
 */
class Curve
{
public:
    virtual ~Curve() {}

    /// Get point on the curve
    virtual Vec2 position(float x) const = 0;
};

#endif // CURVE_H_
