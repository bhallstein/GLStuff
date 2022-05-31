#ifndef Vectors_h
#define Vectors_h

#include "glm_include.hpp"

typedef glm::vec2 v2;
typedef glm::vec3 v3;
typedef glm::vec4 v4;

typedef glm::ivec2 i2;
typedef glm::ivec3 i3;
typedef glm::ivec4 i4;

typedef glm::mat3 m3;
typedef glm::mat4 m4;

typedef glm::quat quat;


inline float angle_between(v2 a, v2 b) {
  a = glm::normalize(a);
  b = glm::normalize(b);

  float det = a.x*b.y - a.y*b.x;
  return atan2(det, glm::dot(a, b));
  // angle = angle < 0 ? 2 * M_PI + angle : angle;
  // return angle == -0. ? 0 : angle;
}


inline float angleBetweenVecAndXYPlane(v3 v) {
  // sin ø = |Aa + Bb + Cc| / sqrt(AA + BB + CC)·sqrt(aa + bb + cc)
  float theta = fabsf(v.z) / glm::length(v);
  return asinf(theta);
}


inline float angleBetweenVecAndYZPlane(v3 v) {
  float theta = fabsf(v.x) / glm::length(v);
  return asinf(theta);
}

#endif
