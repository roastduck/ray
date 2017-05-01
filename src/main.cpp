#include <opencv2/opencv.hpp>
#include "utils.h"
#include "const.h"
#include "surface.h"

int main()
{
    cv::Mat3b canvas(SCREEN_HEIGHT, SCREEN_WIDTH, cv::Vec3b(0, 0, 0));

    auto surfaces = Surface::load(INPUT_FILE);
    for (int i = 0; i < SCREEN_WIDTH; i++)
        for (int j = 0; j < SCREEN_HEIGHT; j++)
        {
            Ray ray(Vec3(0, -1000, 250), Vec3((i - SCREEN_WIDTH * 0.5) * RES_ANGLE, 1, (j - SCREEN_HEIGHT * 0.5) * RES_ANGLE));
            auto interOpt = Surface::findInter(surfaces, ray);
            if (interOpt.isOk())
                canvas(SCREEN_HEIGHT - j - 1, i) = cv::Vec3b(255, 255, 255);
        }

    cv::imwrite(OUTPUT_FILE, canvas);

    return 0;
}

