#include <cassert>
#include <fstream>
#include "boxtree.h"
#include "curve/bezier3.h"
#include "surface/axisymmetric.h"

void Surface::init()
{
    boxTree = std::unique_ptr<BoxTree>(new BoxTree(*this));
}

Optional<InterType> Surface::findInter(const Ray &ray) const
{
    // TODO
}

std::vector< std::unique_ptr<Surface> > Surface::load(const char filename[])
{
    std::ifstream is(filename);

    std::vector< std::unique_ptr<Surface> > ret;
    int name = INVALID;
    while (is >> name)
    {
        switch (Name(name))
        {
        case SYM_BEZIER3: {
            Vec2 p0, p1, p2, p3;
            is >> p0 >> p1 >> p2 >> p3;
            ret.push_back(std::unique_ptr<Surface>(new Axisymmetric(new Bezier3(p0, p1, p2, p3))));
            break;
        }
        default:
            assert(false);
        }
    }
    return ret;
}

