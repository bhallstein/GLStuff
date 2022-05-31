#ifndef Rectangle_h
#define Rectangle_h

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
