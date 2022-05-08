#include "Vectors.hpp"

float dot_product(v2 a, v2 b) {
	return a.x * b.x + a.y * b.y;
}

float dot_product(v3 a, v3 b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

v3 v3_add(v3 a, v3 b) {
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}
v3 v3_subtract(v3 a, v3 b) {
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}

float v2_mod(v2 v) {
	return sqrtf(v.x*v.x + v.y*v.y);
}

float v3_mod(v3 v) {
	return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

v2 v2_normalize(v2 v) {
	float inv_sqrt_len = 1.0 / v2_mod(v);
	return { v.x * inv_sqrt_len, v.y * inv_sqrt_len };
}

float angle_between(v2 a, v2 b) {
  a = v2_normalize(a);
  b = v2_normalize(b);

  float dot = a.x*b.x + a.y*b.y;
  float det = a.x*b.y - a.y*b.x;
  return atan2(det, dot);
  // angle = angle < 0 ? 2 * M_PI + angle : angle;
  // return angle == -0. ? 0 : angle;
}

float angleBetweenVecAndXYPlane(v3 v) {
	// sin ø = |Aa + Bb + Cc| / sqrt(AA + BB + CC)·sqrt(aa + bb + cc)
	float theta = fabsf(v.z) / v3_mod(v);
	return asinf(theta);
}

float angleBetweenVecAndYZPlane(v3 v) {
	float theta = fabsf(v.x) / v3_mod(v);
	return asinf(theta);
}
