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
                    Vec3 st(randInBall(*(emitRandEngine[i]), light->translate, light->radius));
                    ColoredRay ray(
                        light->color,
                        randSemisphere(*(emitRandEngine[i]), st, Vec3(0, 0, -1), 1)
                    );
                    Trace::trace(*(emitRandEngine[i]), surfaces, ray, DEPTH_PER_LIGHT, false,
                        [i](const SurfInterType &inter, const ColoredRay &ray, int depth) {
                            assert(!inter.surf->isLightSource());
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
#pragma omp parallel for schedule(dynamic, 5) // use `dynamic` because tasks are not even
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
            for (int k = 0; k < RAY_PER_PIXEL; k++)
            {
                float eyeX = FILM_WIDTH - (i + randReal(*(collectRandEngine[i]), 0, 1)) * 2 * FILM_WIDTH / SCREEN_WIDTH;
                float eyeZ = FILM_HEIGHT - (j + randReal(*(collectRandEngine[i]), 0, 1)) * 2 * FILM_HEIGHT / SCREEN_HEIGHT;
                float objX = -eyeX * LENS_D2 / LENS_D1;
                float objZ = -eyeZ * LENS_D2 / LENS_D1;
                float lensX = randReal(*(collectRandEngine[i]), -LENS_H, LENS_H);
                float lensZ = randReal(*(collectRandEngine[i]), -LENS_H, LENS_H);
                ColoredRay ray(Vec3(1, 1, 1), Ray(Vec3(lensX, LENS_Y, lensZ), Vec3(objX - lensX, LENS_D2, objZ - lensZ)));

                color_t curColor(0, 0, 0);
                int curWeight(0);
                Trace::trace(*(collectRandEngine[i]), surfaces, ray, DEPTH_PER_PIXEL, true,
                    [i,j,&curColor,&curWeight](const SurfInterType &inter, const ColoredRay &ray, int depth) {
                        assert(!inter.surf->isLightSource());
                        auto photons = inter.surf->photonMap->getKNN(inter.pos, KNN_K);
                        if (photons.empty()) return true;
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
            const color_t &color(screenColor[i][j] * (1.0f / RAY_PER_PIXEL));
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

