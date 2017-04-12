#include <cassert>
#include <fstream>
#include "curve/bezier3.h"
#include "surface/axisymmetric.h"

std::vector<Surface*> Surface::load(const char filename[])
{
    std::ifstream is(filename);

    std::vector<Surface*> ret;
    int name = INVALID;
    while (is >> name)
    {
        switch (Name(name))
        {
        case SYM_BEZIER3: {
            Vec2 p0, p1, p2, p3;
            is >> p0 >> p1 >> p2 >> p3;
            ret.push_back(new Axisymmetric(new Bezier3(p0, p1, p2, p3)));
            break;
        }
        default:
            assert(false);
        }
    }
    return ret;
}

