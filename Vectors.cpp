//
//  Vectors.cpp
//  ComponentTest
//
//  Created by Ben on 16/11/2015.
//  Copyright © 2015 Ben. All rights reserved.
//

#include "Vectors.h"


float dotProduct(const v2 &a, const v2 &b) {
	return a.x * b.x + a.y * b.y;
}
float dotProduct(const v3 &a, const v3 &b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


v3 v3_plus(const v3 &a, const v3 &b) {
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}
v3 v3_minus(const v3 &a, const v3 &b) {
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}

float v2_mod(const v2 &v) {
	return sqrtf(v.x*v.x + v.y*v.y);
}
float v3_mod(const v3 &v) {
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

v2 v2_normalize(const v2 &v) {
	float inv_sqrt_len = 1.0 / v2_mod(v);
	return { v.x * inv_sqrt_len, v.y * inv_sqrt_len };
}

float angleBetweenVecAndXYPlane(const v3 &v) {
	// sin ø = |Aa + Bb + Cc| / sqrt(AA + BB + CC)·sqrt(aa + bb + cc)
	float theta = fabsf(v.z) / v3_mod(v);
	return asinf(theta);
}
float angleBetweenVecAndYZPlane(const v3 &v) {
	float theta = fabsf(v.x) / v3_mod(v);
	return asinf(theta);
}

//float crossProduct(v3a , v3 b) {
//	
//}
//float crossProduct(Triangle) {
//	
//}


