#include <cstring>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include "trace.h"
#include "utils.h"
#include "const.h"
#include "surface.h"
#include "photonmap.h"

std::vector<ColoredRay> loadLights()
{
    std::ifstream is(INPUT_LIGHTS_FILE);
    std::vector<ColoredRay> ret;
    ColoredRay cur;
    while (is >> cur.color >> cur.ray.st >> cur.ray.dir)
        ret.push_back(cur);
    return ret;
}

color_t screenColor[SCREEN_WIDTH][SCREEN_HEIGHT];
float screenWeight[SCREEN_WIDTH][SCREEN_HEIGHT];

int main()
{
    srand(0);
    cv::Mat3b canvas(SCREEN_HEIGHT, SCREEN_WIDTH, cv::Vec3b(0, 0, 0));
    memset(screenColor, 0, sizeof screenColor);
    memset(screenWeight, 0, sizeof screenWeight);

    auto surfaces = Surface::load(INPUT_OBJECTS_FILE);
    auto lights = loadLights();

    for (const ColoredRay &ray : lights)
        for (int i = 0; i < RAY_PER_LIGHT; i++)
            Trace::trace(surfaces, ray, DEPTH_PER_LIGHT, [](const Surface::SurfInterType &inter, const ColoredRay &ray) {
                const_cast<Surface*>(inter.surf)->photonMap.addRay(ColoredRay(ray.color, Ray(inter.pos, ray.ray.dir))); // `st` must be on the surface
            });

    for (const auto &surf : surfaces)
        surf->photonMap.buildTree();

    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            ColoredRay ray(
                Vec3(1, 1, 1),
                Ray(Vec3(0, -1000, 0), Vec3((i - SCREEN_WIDTH * 0.5) * RES_ANGLE, 1, (j - SCREEN_HEIGHT * 0.5) * RES_ANGLE))
            );
            for (int k = 0; k < RAY_PER_PIXEL; k++)
                Trace::trace(surfaces, ray, DEPTH_PER_PIXEL, [i,j](const Surface::SurfInterType &inter, const ColoredRay &ray) {
                    auto photons = inter.surf->photonMap.getKNN(inter.pos, KNN_K);
                    float r2 = 0;
                    color_t color(0, 0, 0);
                    for (const ColoredRay &photon : photons)
                    {
                        color += multiple(photon.color, Trace::colorFactor(photon.ray.dir, ray.ray.dir, inter));
                        r2 = std::max(r2, (inter.pos - photon.ray.st).dist2());
                    }
                    color *= 1.0f / r2;
                    screenColor[i][j] += multiple(color, ray.color);
                    screenWeight[i][j] += 1;
                });
        }
    // TODO: check if lights directly go into screen or the contrary
    
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            color_t color(screenColor[i][j] * (1.0f / screenWeight[i][j]));
            canvas(SCREEN_HEIGHT - j - 1, i) = cv::Vec3b(color.z, color.y, color.x); // BGR
        }
    cv::imwrite(OUTPUT_FILE, canvas);

    return 0;
}

