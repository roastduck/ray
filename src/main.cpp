#include <cstring>
#include <vector>
#include <fstream>
#include <random>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "trace.h"
#include "utils.h"
#include "const.h"
#include "surface.h"
#include "photonmap.h"
#include "surface/lightsource.h"

void emit(const std::vector< std::unique_ptr<Surface> > &surfaces)
{
    const int POOL_SIZE = 1024;
    static std::vector< std::pair<const Surface*, ColoredRay> > rayQueue[POOL_SIZE];
    static std::unique_ptr<std::default_random_engine> emitRandEngine[POOL_SIZE]; /// Thread safe

    for (int i = 0; i < POOL_SIZE; i++)
        emitRandEngine[i] = std::unique_ptr<std::default_random_engine>(new std::default_random_engine(rand()));

    std::cout << "Emitting photons" << std::endl;
    for (const auto &surf : surfaces)
        if (surf->isLightSource())
        {
            auto light = dynamic_cast<const LightSource*>(surf.get());
            int iterNum = RAY_PER_LIGHT / POOL_SIZE;
            for (int k = 0; k < iterNum; k++)
            {
#pragma omp parallel for
                for (int i = 0; i < POOL_SIZE; i++)
                {
                    assert(rayQueue[i].empty());
                    ColoredRay ray(
                        light->color,
                        randSemisphere(*(emitRandEngine[i]), randInBall(*(emitRandEngine[i]), light->translate, light->radius), light->direction)
                    );
                    Trace::trace(*(emitRandEngine[i]), surfaces, ray, DEPTH_PER_LIGHT,
                        [i](const SurfInterType &inter, const ColoredRay &ray, int depth) {
                            if (inter.surf->isLightSource())
                                return false;
                            rayQueue[i].push_back(std::make_pair(inter.surf, ColoredRay(ray.color, Ray(inter.pos, ray.ray.dir))));
                            // `st` must be on the surface
                            return true;
                        });
                }
                for (int i = 0; i < POOL_SIZE; i++)
                {
                    for (const auto &item : rayQueue[i])
                        const_cast<Surface*>(item.first)->photonMap->addRay(item.second);
                    rayQueue[i].clear();
                }
            }
        }

    for (int i = 0; i < POOL_SIZE; i++)
        emitRandEngine[i] = nullptr;
}

void build(const std::vector< std::unique_ptr<Surface> > &surfaces)
{
    std::cout << "Building kd-tree" << std::endl;
    for (const auto &surf : surfaces)
        if (!surf->isLightSource())
            surf->photonMap->buildTree();
}

void collect(const std::vector< std::unique_ptr<Surface> > &surfaces, cv::Mat3b &canvas)
{
    static color_t screenColor[SCREEN_WIDTH][SCREEN_HEIGHT];
    static std::unique_ptr<std::default_random_engine> collectRandEngine[SCREEN_WIDTH];

    for (int i = 0; i < SCREEN_WIDTH; i++)
        collectRandEngine[i] = std::unique_ptr<std::default_random_engine>(new std::default_random_engine(rand()));
    memset(screenColor, 0, sizeof screenColor);

    std::cout << "Tracing sight" << std::endl;
#pragma omp parallel for
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            for (int k = 0; k < RAY_PER_PIXEL; k++)
            {
                float _i = i + randReal(*(collectRandEngine[i]), -0.5, 0.5);
                float _j = j + randReal(*(collectRandEngine[i]), -0.5, 0.5);
                ColoredRay ray(
                    Vec3(1, 1, 1),
                    Ray(Vec3(0, -1000, 0), Vec3((_i - SCREEN_WIDTH * 0.5) * RES_ANGLE, 1, (_j - SCREEN_HEIGHT * 0.5) * RES_ANGLE))
                );
                color_t curColor(0, 0, 0);
                int curWeight(0);
                Trace::trace(*(collectRandEngine[i]), surfaces, ray, DEPTH_PER_PIXEL,
                    [i,j,&curColor,&curWeight](const SurfInterType &inter, const ColoredRay &ray, int depth) {
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
                        color *= 1.0f / (PI * r2);
                        curColor += multiple(color, ray.color), curWeight++;
                        return true;
                    });
                if (curWeight)
                    screenColor[i][j] += curColor * (1.0f / curWeight);
            }

    for (int i = 0; i < SCREEN_WIDTH; i++)
        collectRandEngine[i] = nullptr;

    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            const color_t &color(screenColor[i][j]);
            unsigned char R(std::min<int>(255, color.x * 256));
            unsigned char G(std::min<int>(255, color.y * 256));
            unsigned char B(std::min<int>(255, color.z * 256));
            canvas(SCREEN_HEIGHT - j - 1, i) = cv::Vec3b(B, G, R);
        }
}


int main()
{
    srand(0); // Use rand() as seed for random engines
    cv::Mat3b canvas(SCREEN_HEIGHT, SCREEN_WIDTH, cv::Vec3b(0, 0, 0));

    auto surfaces = Surface::load(INPUT_OBJECTS_FILE);

    emit(surfaces);
    build(surfaces);
    collect(surfaces, canvas);

    cv::imwrite(OUTPUT_FILE, canvas);
    return 0;
}

