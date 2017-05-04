#include <cstring>
#include <vector>
#include <fstream>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "trace.h"
#include "utils.h"
#include "const.h"
#include "surface.h"
#include "photonmap.h"
#include "surface/lightsource.h"

color_t screenColor[SCREEN_WIDTH][SCREEN_HEIGHT];
float screenWeight[SCREEN_WIDTH][SCREEN_HEIGHT];

int main()
{
    srand(0);
    cv::Mat3b canvas(SCREEN_HEIGHT, SCREEN_WIDTH, cv::Vec3b(0, 0, 0));
    memset(screenColor, 0, sizeof screenColor);
    memset(screenWeight, 0, sizeof screenWeight);

    auto surfaces = Surface::load(INPUT_OBJECTS_FILE);

    std::cout << "Emitting photons" << std::endl;
    for (const auto &surf : surfaces)
        if (surf->isLightSource())
        {
            auto light = dynamic_cast<const LightSource*>(surf.get());
            for (int i = 0; i < RAY_PER_LIGHT; i++)
            {
                ColoredRay ray(light->color, randSemisphere(randInBall(light->translate, light->radius), light->direction));
                Trace::trace(surfaces, ray, DEPTH_PER_LIGHT, [](const SurfInterType &inter, const ColoredRay &ray, int depth) {
                    if (inter.surf->isLightSource())
                        return false;
                    const_cast<Surface*>(inter.surf)->photonMap->addRay(ColoredRay(ray.color, Ray(inter.pos, ray.ray.dir)));
                    // `st` must be on the surface
                    return true;
                });
            }
        }

    std::cout << "Building kd-tree" << std::endl;
    for (const auto &surf : surfaces)
        if (!surf->isLightSource())
            surf->photonMap->buildTree();

    std::cout << "Tracing sight" << std::endl;
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            for (int k = 0; k < RAY_PER_PIXEL; k++)
            {
                float _i = i + rand01() - 0.5;
                float _j = j + rand01() - 0.5;
                ColoredRay ray(
                    Vec3(1, 1, 1),
                    Ray(Vec3(0, -1000, 0), Vec3((_i - SCREEN_WIDTH * 0.5) * RES_ANGLE, 1, (_j - SCREEN_HEIGHT * 0.5) * RES_ANGLE))
                );
                Trace::trace(surfaces, ray, DEPTH_PER_PIXEL, [i,j](const SurfInterType &inter, const ColoredRay &ray, int depth) {
                    if (inter.surf->isLightSource())
                    {
                        auto light = dynamic_cast<const LightSource*>(inter.surf);
                        screenColor[i][j] += multiple(light->color, ray.color) * float(depth + 1);
                        return false;
                    }
                    auto photons = inter.surf->photonMap->getKNN(inter.pos, KNN_K);
                    float r2 = 0;
                    color_t color(0, 0, 0);
                    for (const ColoredRay &photon : photons)
                    {
                        color += multiple(photon.color, Trace::colorFactor(photon.ray.dir, ray.ray.dir, inter));
                        r2 = std::max(r2, (inter.pos - photon.ray.st).dist2());
                    }
                    color *= PI / r2;
                    screenColor[i][j] += multiple(color, ray.color);
                    return true;
                });
            }
            screenWeight[i][j] = RAY_PER_PIXEL * (DEPTH_PER_PIXEL + 1);
        }
    
    std::cout << "Final output" << std::endl;
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            if (screenWeight[i][j]) // Avoid dividing by 0
            {
                color_t color(screenColor[i][j] * (1.0f / screenWeight[i][j]));
                unsigned char R(std::min<int>(255, color.x * 256));
                unsigned char G(std::min<int>(255, color.y * 256));
                unsigned char B(std::min<int>(255, color.z * 256));
                canvas(SCREEN_HEIGHT - j - 1, i) = cv::Vec3b(B, G, R);
            }
    cv::imwrite(OUTPUT_FILE, canvas);

    return 0;
}

