//
//  Vectors.hpp
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#ifndef Vectors_hpp
#define Vectors_hpp

#include "Primitives.h"
#include <cmath>

float dotProduct(const v2 &, const v2 &);
float dotProduct(const v3 &, const v3 &);

v3 v3_plus(const v3 &a, const v3 &b);
v3 v3_minus(const v3 &a, const v3 &b);

float v2_mod(const v2 &v);
float v3_mod(const v3 &v);

v2 v2_normalize(const v2 &v);

float angleBetweenVecAndXYPlane(const v3 &v);
float angleBetweenVecAndYZPlane(const v3 &v);

//float crossProduct(const v3 &, const v3 &);
//float crossProduct(const Triangle &);


#endif
