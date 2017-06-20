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
const int OUTPUT_MESH_1D_NUM = 100;

// MATH
const float EPS = 1e-3;

// RESOLUTION
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

// QUALITY
const int DEPTH_PER_LIGHT = 5;
const int RAY_PER_LIGHT = 10000000;
const int DEPTH_PER_PIXEL = 5;
const int RAY_PER_PIXEL = 250;
const int KNN_K = 100;

// LENS
//  eye        lens          object
// --+-----------|-------F-----+-----
//   |----D1-----|------D2-----|
constexpr const float LENS_Y = -750;
constexpr const float LENS_F = 400;
constexpr const float LENS_H = LENS_F / 9 / 2; /// Aperture (diameter) / 2
constexpr const float LENS_D2 = 700;
constexpr const float LENS_D1 = LENS_D2 * LENS_F / (LENS_D2 - LENS_F);
constexpr const float FILM_WIDTH = 266.5 / (-266.5 - LENS_Y) * LENS_D1;
constexpr const float FILM_HEIGHT = FILM_WIDTH / SCREEN_WIDTH * SCREEN_HEIGHT;

#endif // CONST_H_

