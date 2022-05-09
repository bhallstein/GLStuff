#ifndef Vectors_h
#define Vectors_h

#include "Primitives.hpp"
#include <cmath>

float dot_product(v2, v2);
float dot_product(v3, v3);

v3 v3_add(v3, v3);
v3 v3_subtract(v3, v3);

float v2_mod(v2);
float v3_mod(v3);

v2 v2_normalize(v2);

float angle_between(v2, v2);
float angleBetweenVecAndXYPlane(v3);
float angleBetweenVecAndYZPlane(v3);

#endif
