#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <opencv2/opencv.hpp>
#include "utils.h"

class Texture
{
private:
    cv::Mat3b mat;
    Texture(const char filename[]) : mat(cv::imread(filename)) {}

public:
    color_t getColor(float u, float v)
    {
        int i(u * mat.cols), j(v * mat.rows);
        cv::Vec3b vec(mat(mat.rows - j - 1, i));
        return color_t(vec[2] / 256.0f, vec[1] / 256.0f, vec[0] / 256.0f); // BGR
    }
    
    static Texture wallpaper, apple1, apple2;
};

#endif // TEXTURE_H_

