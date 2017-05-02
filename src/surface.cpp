#include <cassert>
#include <fstream>
#include "const.h"
#include "curve/bezier3.h"
#include "surface/axisymmetric.h"

void Surface::init()
{
    boxTree = std::unique_ptr<BoxTree>(new BoxTree(*this));
}

Optional<Surface::SurfInterType> Surface::findInter(const Ray &ray) const
{
    assert(boxTree);
    auto boxInterOpt = boxTree->findInter(ray);
    if (! boxInterOpt.isOk())
        return None();

    const auto &boxInter = boxInterOpt.ok();

    float u((boxInter.first->u1 + boxInter.first->u2) * 0.5);
    float v((boxInter.first->v1 + boxInter.first->v2) * 0.5);
    float t(boxInter.second.second);
    Vec3 s(position(u, v)), c(boxInter.second.first), f(c - s);
    Vec3 su(derivativeU(u, v)), sv(derivativeV(u, v));
    const Vec3 &ct(ray.dir);

    for (int i = 0; i < 5; i++)
    {
        u -= dot(ct, cross(sv, f)) / dot(ct, cross(sv, -su)); // 1
        s = position(u, v), f = c - s, su = derivativeU(u, v), sv = derivativeV(u, v);

        v -= dot(ct, cross(su, f)) / dot(ct, cross(su, -sv)); // 2
        s = position(u, v), f = c - s, su = derivativeU(u, v), sv = derivativeV(u, v);

        t -= dot(su, cross(sv, f)) / dot(su, cross(sv, ct)); // 3
        c = ray.st + t * ray.dir, f = c - s;

        if (f.dist2() < EPS * EPS)
        {
            if (t > EPS) // Move out of original position
                return SurfInterType(this, t, u, v, s, cross(su, sv));
            else
                return None();
        }
    }
    return None();
}

std::vector< std::unique_ptr<Surface> > Surface::load(const char filename[])
{
    std::ifstream is(filename);

    std::vector< std::unique_ptr<Surface> > ret;
    int name = INVALID;
    int material = Material::INVALID;
    while (is >> name >> material)
    {
        switch (Name(name))
        {
        case SYM_BEZIER3: {
            Vec2 p0, p1, p2, p3;
            is >> p0 >> p1 >> p2 >> p3;
            auto surf = std::unique_ptr<Surface>(
                new Axisymmetric(Material::byName(Material::MaterialName(material)), new Bezier3(p0, p1, p2, p3))
            );
            surf->init();
            ret.push_back(std::move(surf));
            break;
        }
        default:
            assert(false);
        }
    }
    return ret;
}

