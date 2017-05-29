#include <cmath>
#include <cassert>
#include <fstream>
#include "const.h"
#include "curve/bezier3.h"
#include "surface/squarexy.h"
#include "surface/squareyz.h"
#include "surface/squarezx.h"
#include "surface/lightsource.h"
#include "surface/axisymmetric.h"

void Surface::init()
{
    boxTree = std::unique_ptr<BoxTree>(new BoxTree(*this));
}

Optional<SurfInterType> Surface::findInter(const Ray &ray) const
{
    assert(boxTree);
    return boxTree->findInter(ray);
}

std::vector< std::unique_ptr<Surface> > Surface::load(const char filename[])
{
    enum Name {
        SYM_BEZIER3 = 1,
        SQUAREXY = 2,
        SQUAREYZ = 3,
        SQUAREZX = 4,
        LIGHT_SOURCE = -2,
        INVALID = -1
    };

    std::ifstream is(filename);

    std::vector< std::unique_ptr<Surface> > ret;
    int name = INVALID;
    int material = Material::INVALID;
    while (is >> name)
    {
        std::unique_ptr<Surface> surf(nullptr);
        switch (Name(name))
        {
        case SQUAREXY: {
            float edgeX, edgeY;
            is >> edgeX >> edgeY;
            surf = std::unique_ptr<Surface>(new SquareXY(edgeX, edgeY));
            break;
        }
        case SQUAREYZ: {
            float edgeY, edgeZ;
            is >> edgeY >> edgeZ;
            surf = std::unique_ptr<Surface>(new SquareYZ(edgeY, edgeZ));
            break;
        }
        case SQUAREZX: {
            float edgeX, edgeZ;
            is >> edgeX >> edgeZ;
            surf = std::unique_ptr<Surface>(new SquareZX(edgeX, edgeZ));
            break;
        }
        case SYM_BEZIER3: {
            Vec2 p0, p1, p2, p3;
            is >> p0 >> p1 >> p2 >> p3;
            surf = std::unique_ptr<Surface>(new Axisymmetric(new Bezier3(p0, p1, p2, p3)));
            break;
        }
        case LIGHT_SOURCE: {
            Vec3 direction;
            color_t color;
            float radius;
            is >> radius >> color >> direction;
            color *= 1.0f / RAY_PER_LIGHT;
            surf = std::unique_ptr<Surface>(new LightSource(radius, color, direction));
            break;
        }
        default:
            assert(false);
        }
        if (!surf->isLightSource())
        {
            is >> material;
            surf->material = std::unique_ptr<Material>(new Material(Material::byName(Material::MaterialName(material))));
            surf->photonMap = std::unique_ptr<PhotonMap>(new PhotonMap(*surf));
        }
        is >> surf->translate;
        surf->init();
        ret.push_back(std::move(surf));
    }
    return ret;
}

