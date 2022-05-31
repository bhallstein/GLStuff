#ifndef Primitives_h
#define Primitives_h

#include <vector>
#include "vectors.hpp"

// NB: default opengl winding order is CCW = front face

struct NormalMode {
  enum T {
    Flat,
    Smooth,
  };
};


struct Primitive3D {
  std::vector<v3> vertices;
  std::vector<v3> normals;
};


inline std::vector<v2> to_2d_primitive(const Primitive3D &p) {
  std::vector<v2> out;
  for (const v3 &point : p.vertices) {  // z coords discarded
    out.push_back(point);
  }
  return out;
}


inline std::vector<v3> mk_flat_normals(const std::vector<v3> &vertices) {
  std::vector<v3> normals;
  size_t n = vertices.size() / 3;
  for (size_t i=0; i < n; ++i) {
    v3 p1 = vertices[i*3];
    v3 p2 = vertices[i*3 + 1];
    v3 p3 = vertices[i*3 + 2];
    v3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
    normals.push_back(normal);
    normals.push_back(normal);
    normals.push_back(normal);
  }
  return normals;
}

#include "operators.hpp"
#include "triangles.hpp"
#include "rectangle.hpp"
#include "cube.hpp"
#include "square-pyramid.hpp"
#include "jewel.hpp"

#endif
