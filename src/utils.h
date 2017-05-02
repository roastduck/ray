/** Miscellaneous
 *  This header will include all the headers in utils/ folder
 *  Including vectors, boxes, helper functions, mathematical constants
 */

#ifndef UTILS_H_
#define UTILS_H_

#include "utils/vec.h"
#include "utils/box.h"
#include "utils/ray.h"
#include "utils/math.h"
#include "utils/random.h"
#include "utils/optional.h"

inline bool inrange(float x, float l, float r)
{
    return x >= l && x <= r;
}

#endif // UTILS_H_

