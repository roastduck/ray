/** Random distributions
 *  Functions here uses C++11 random generators to provide thread-safe feature
 */

#include <cmath>
#include <random>
#include "ray.h"
#include "./math.h"

template <class URNG>
inline float randReal(URNG &g, float a, float b)
{
    return std::uniform_real_distribution<float>(a, b)(g);
}

/** Random ray goes from `st`, with the direction in the semi sphere around `norm`
 *  For each ray `x`, its weight is cos(x . norm)^n
 */
template <class URNG>
inline Ray randSemisphere(URNG &g, const Vec3 &st, const Vec3 &norm, int n = 0)
{
    float phi = randReal(g, 0, 2 * PI);
    float theta = acosf(n ? powf(randReal(g, 0, 1), 1.0f / (n + 1)) : randReal(g, 0, 1));
    float phi0 = atan2f(norm.y, norm.x);
    float theta0 = atan2f(sqrtf(norm.x * norm.x + norm.y * norm.y), norm.z);
    Vec3 dir = Vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta)).rotate(phi0, theta0);
    return Ray(st, dir);
}

/// Random point on a ball
template <class URNG>
inline Vec3 randInBall(URNG &g, const Vec3 &center, float radius)
{
    float phi = randReal(g, 0, 2 * PI);
    float theta = acosf(randReal(g, -1, 1));
    return center + Vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta)) * radius;
}

