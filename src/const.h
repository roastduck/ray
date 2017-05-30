#ifndef CONST_H_
#define CONST_H_

#include <cmath>
#include "utils.h"

// File names
const char INPUT_OBJECTS_FILE[] = "objects.txt";
const char OUTPUT_FILE[] = "output.png";
const char MESH_FILE[] = "mesh.obj";

// MESH
const int MESH_1D_NUM = 25;

// MATH
const float EPS = 1e-3;

// RESOLUTION
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float RES_ANGLE = 1.f / SCREEN_WIDTH; /// Resolution angle of the central pixel. Horizontal opening angle = 45 dgr.

// QUALITY
const int DEPTH_PER_LIGHT = 5;
const int RAY_PER_LIGHT = 25000000;
const int DEPTH_PER_PIXEL = 2;
const int RAY_PER_PIXEL = 250;
const int KNN_K = 50;

#endif // CONST_H_

