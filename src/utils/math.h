#ifndef UTILS_MATH_H_
#define UTILS_MATH_H_

#include <cmath>
#include <limits>

template<class T> T sqr(T x) { return x * x; }

const float INF = std::numeric_limits<float>::infinity();
const float PI = acosf(-1);

#endif // UTILS_MATH_H_

