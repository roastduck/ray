#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cassert>
#include "utils.h"

struct Material
{
    float Kd; /// Diffuse reflection rate
    float Ks; /// Specular relection rate
    float Ktd; /// Diffuse transition rate
    float Kts; /// Specular transition rate
    float Rn; /// Refrection rate
    int Sn; /// Smoothness index (exponent)
    color_t Creflec; /// Reflection color factor. Actual color = Creflec * (Kd or Ks)
    color_t Ctrans; /// Transition color factor. Actual color = Ctrans * (Ktd or Kts)

    enum MaterialName
    {
        GLASS = 0,
        PLASTIC = 1,
        INVALID = -1
    };

    static Material byName(MaterialName name)
    {
        switch (name)
        {
        case GLASS:
            return { 0.001, 0.2, 0.001, 0.96, 1.6, 15, color_t(1.0, 1.0, 1.0), color_t(1.0, 1.0, 1.0) };
        case PLASTIC:
            return { 0.50, 0.10, 0.00, 0.00, 1.0, 3, color_t(1.1, 1.0, 0.9), color_t(1.0, 1.0, 1.0) };
        default:
            assert(false);
        }
    }
};

#endif // MATERIAL_H_

