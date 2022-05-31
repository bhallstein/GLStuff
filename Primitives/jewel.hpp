#ifndef Jewel_h
#define Jewel_h

inline Primitive3D Jewel() {
  static const v3 rf  = {  0.5, 0,  0.5 };  // right front, etc.
  static const v3 lf  = { -0.5, 0,  0.5 };
  static const v3 rb  = {  0.5, 0, -0.5 };
  static const v3 lb  = { -0.5, 0, -0.5 };
  static const v3 top = { 0,  1, 0 };
  static const v3 btm = { 0, -1, 0 };

  static const std::vector<v3> vertices = {
    btm, rf, lf,  // bottom front
    btm, lb, rb,  // bottom back
    btm, lf, lb,  // bottom left
    btm, rb, rf,  // bottom right
    top, lf, rf,  // top front
    top, rb, lb,  // top back
    top, lb, lf,  // top left
    top, rf, rb,  // top right
  };

  static const Primitive3D jewel = {
    .vertices = vertices,
    .normals = mk_flat_normals(vertices),
  };

  return jewel;
}

#endif
