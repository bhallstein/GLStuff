#ifndef Triangles_h
#define Triangles_h

inline Primitive3D EquilateralTriangle() {
  static const v3 n = {0, 0, 1};
  static const Primitive3D eqtr = {
    .vertices = {
      { 0.0,  0.433, 0},
      {-0.5, -0.433, 0},
      { 0.5, -0.433, 0},
    },
    .normals = {
      n, n, n,
    },
  };
  return eqtr;
};

#endif
