#pragma once

#include "Vector3D.h"

#if defined __linux__ || defined __APPLE__ 
// "Compiled for Linux
#else 
// Windows doesn't define these values by default, Linux does
#define M_PI 3.141592653589793 
#define INFINITY 1e8 
#endif 

const float RAY_MIN_DIST = 1e-4;
const float RAY_MAX_DIST = 1e5;
const float HIT_POINT_BIAS = 1e-4;
const int MAX_RAY_DEPTH = 5;
const Vec3f ENVIRONMENT_LIGHT = Vec3f(0.3);
const float CONST_ETA = 1.1f;