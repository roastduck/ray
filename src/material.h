#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <cassert>
#include "utils.h"
#include "texture.h"

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
    Texture *texture;

    color_t getCreflec(float u, float v) const { return texture ? texture->getColor(u, v) : Creflec; }

    enum MaterialName
    {
        GLASS = 0,
        PAPER = 1,
        WALLPAPER = 2,
        FROSTED_GLASS = 3,
        MATEL = 4,
        APPLE = 5,
        INVALID = -1
    };

    static Material byName(MaterialName name)
    {
        switch (name)
        {
        case GLASS:
            return { 0.00, 0.00, 0.00, 1.00, 1.6, 500, color_t(1.0, 1.0, 1.0), color_t(1.0, 1.0, 1.0), NULL };
        case PAPER:
            return { 0.50, 0.10, 0.00, 0.00, 1.0, 7, color_t(1.1, 1.0, 0.9), color_t(1.0, 1.0, 1.0), NULL };
        case WALLPAPER:
            return { 0.50, 0.10, 0.00, 0.00, 1.0, 7, color_t(0.0, 0.0, 0.0), color_t(1.0, 1.0, 1.0), &Texture::wallpaper };
        case FROSTED_GLASS:
            return { 0.001, 0.02, 0.001, 0.96, 1.6, 15, color_t(1.0, 1.0, 1.0), color_t(1.0, 1.0, 1.0), NULL };
        case MATEL:
            return { 0.15, 0.80, 0.00, 0.00, 1.0, 100, color_t(1.0, 1.0, 1.0), color_t(1.0, 1.0, 1.0), NULL };
        case APPLE:
            return { 0.30, 0.30, 0.00, 0.00, 1.0, 7, color_t(0.0, 0.0, 0.0), color_t(1.0, 1.0, 1.0), &Texture::apple };
        default:
            assert(false);
        }
    }
};

#endif // MATERIAL_H_

