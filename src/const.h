#ifndef CONST_H_
#define CONST_H_

#include <cmath>

// File names
const char INPUT_FILE[] = "input.txt";
const char OUTPUT_FILE[] = "output.png";
const char MESH_FILE[] = "mesh.obj";

// MESH
const int MESH_1D_NUM = 25;

// MATH
const float EPS = 1e-3;

// RESOLUTION
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const float RES_ANGLE = 1.f / SCREEN_WIDTH; /// Resolution angle of the central pixel. Horizontal opening angle = 45 dgr.

#endif // CONST_H_

