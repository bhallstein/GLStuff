#ifndef Rectangle_h
#define Rectangle_h

inline Primitive2D Rectangle2D() {
  static const v2 tl = {-0.5,  0.5};
  static const v2 tr = { 0.5,  0.5};
  static const v2 bl = {-0.5, -0.5};
  static const v2 br = { 0.5, -0.5};
  static const Primitive2D rect = { tr, tl, bl, bl, br, tr };
  return rect;
}

inline Primitive2D Rectangle2DTextureCoords() {
  static const Primitive2D tcs = Rectangle2D() + 0.5f;
  return tcs;
}

inline Primitive3D Rectangle() {
  static const v3 up = { 0, 1, 0};
  static const v3 rf = { 0.5, 0,  0.5};
  static const v3 lf = {-0.5, 0,  0.5};
  static const v3 rb = { 0.5, 0, -0.5};
  static const v3 lb = {-0.5, 0, -0.5};
  static const Primitive3D rect = {
    .vertices = {rb, rf, lf, lf, lb, rb},
    .normals = {up, up, up, up, up, up},
  };
  return rect;
}

#endif
