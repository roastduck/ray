#ifndef RAY_H_
#define RAY_H_

#include "vec.h"

struct Ray
{
    Vec3 st; /// Starting point
    Vec3 dir; /// Direction

    Ray(const Vec3 &_st, const Vec3 &_dir)
        : st(_st), dir(_dir) {}
};

typedef Vec3 color_t;

struct ColoredRay
{
    color_t color;
    Ray ray;
    ColoredRay(const color_t &_color, const Ray &_ray)
        : color(_color), ray(_ray) {}
};

#endif // RAY_H_

