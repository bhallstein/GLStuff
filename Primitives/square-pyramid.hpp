#ifndef SquarePyramid_h
#define SquarePyramid_h

inline Primitive3D SquarePyramid() {
  static const v3 rf  = {  0.5, 0,  0.5 };  // right front, etc.
  static const v3 lf  = { -0.5, 0,  0.5 };
  static const v3 rb  = {  0.5, 0, -0.5 };
  static const v3 lb  = { -0.5, 0, -0.5 };
  static const v3 apex = { 0, 1, 0 };

  static const std::vector<v3> vertices = {
    rf, lf, lb, lb, rb, rf,  // Base
    apex, lf, rf,  // front
    apex, rb, lb,  // back
    apex, lb, lf,  // left
    apex, rf, rb,  // right
  };

  static const Primitive3D sq_pyramid = {
    .vertices = vertices,
    .normals = mk_flat_normals(vertices),
  };

  return sq_pyramid;
}

#endif
