#include <cmath>
#include <cstdlib>
#include "ray.h"
#include "./math.h"

inline float rand01()
{
    return float(rand()) / RAND_MAX;
}

/** Random ray goes from `st`, with the direction in the semi sphere around `norm`
 *  For each ray `x`, its weight is cos(x . norm)^n
 */
inline Ray randSemisphere(const Vec3 &st, const Vec3 &norm, int n = 1)
{
    float phi = rand01() * 2 * PI;
    float theta = acosf(n ? powf(rand01(), 1.0f / (n + 1)) : rand01());
    float phi0 = atan2f(norm.y, norm.x);
    float theta0 = atan2f(sqrtf(norm.x * norm.x + norm.y * norm.y), norm.z);
    Vec3 dir = Vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta)).rotate(phi0, theta0);
    return Ray(st, dir);
}

/// Random point on a ball
inline Vec3 randInBall(const Vec3 &center, float radius)
{
    float phi = rand01() * 2 * PI;
    float theta = rand01() * PI;
    return center + Vec3(sinf(theta) * cosf(phi), sinf(theta) * sinf(phi), cosf(theta)) * radius;
}

