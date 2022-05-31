#ifndef Cube_h
#define Cube_h

inline Primitive3D Cube() {
  static const v3 trf = { 1,  1,  1};  // Top Right Front, etc.
  static const v3 tlf = {-1,  1,  1};
  static const v3 blf = {-1, -1,  1};
  static const v3 brf = { 1, -1,  1};
  static const v3 trb = { 1,  1, -1};
  static const v3 tlb = {-1,  1, -1};
  static const v3 blb = {-1, -1, -1};
  static const v3 brb = { 1, -1, -1};

  static const std::vector<v3> vertices = {
    trf, tlf, blf, blf, brf, trf,  // front
    tlb, trb, brb, brb, blb, tlb,  // back
    tlf, tlb, blb, blb, blf, tlf,  // left
    trb, trf, brf, brf, brb, trb,  // right
    trb, tlb, tlf, tlf, trf, trb,  // top
    blb, brb, brf, brf, blf, blb,  // bottom
  };

  static const Primitive3D cube = {
    .vertices = vertices,
    .normals = mk_flat_normals(vertices),  // only support flat normals as smoothed
  };                                       // will look terrible

  return cube;
}

#endif
