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

    std::cout << "Emitting photons" << std::endl;
    for (const ColoredRay &light : lights)
        for (int i = 0; i < RAY_PER_LIGHT; i++)
        {
            ColoredRay ray(light.color, randSemisphere(light.ray.st, light.ray.dir));
            Trace::trace(surfaces, ray, DEPTH_PER_LIGHT, [](const Surface::SurfInterType &inter, const ColoredRay &ray) {
                const_cast<Surface*>(inter.surf)->photonMap.addRay(ColoredRay(ray.color, Ray(inter.pos, ray.ray.dir))); // `st` must be on the surface
            });
        }

    std::cout << "Building kd-tree" << std::endl;
    for (const auto &surf : surfaces)
        surf->photonMap.buildTree();

    std::cout << "Tracing sight" << std::endl;
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
                    color *= PI / r2;
                    screenColor[i][j] += multiple(color, ray.color);
                    screenWeight[i][j] += 1;
                });
        }
    // TODO: check if lights directly go into screen or the contrary
    
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

