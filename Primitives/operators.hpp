#ifndef Operators_h
#define Operators_h

template<class T, class U>
inline std::vector<T> operator+(const std::vector<T> &vertices, U translation) {
  std::vector<T> out;
  for (const T &point : vertices) {
    out.push_back(point + translation);
  }
  return out;
}

inline std::vector<v2> operator*(const std::vector<v2> &vertices, float scale) {
  std::vector<v2> out;
  for (const v2 &point : vertices) {
    out.push_back(point * scale);
  }
  return out;
}

inline std::vector<v3> operator*(const std::vector<v3> &vertices, float scale) {
  std::vector<v3> out;
  for (const v3 &point : vertices) {
    out.push_back(point * scale);
  }
  return out;
}


template<class T>
T vector_scale(T vec, T scale) {
  T out;
  auto dimensions = vec.length();
  for (auto i = 0; i < dimensions; ++i) {
    out[i] = vec[i] * scale[i];
  }
  return out;
}

template<class T>
inline std::vector<T> operator*(const std::vector<T> &vertices, T scale) {
  std::vector<T> out;
  for (const T &point : vertices) {
    out.push_back(vector_scale(point, scale));
  }
  return out;
}

//inline std::vector<v3> operator*(const std::vector<v3> vertices, v3 scale) {
//  std::vector<v3> out;
//  for (const v3 &point : vertices) {
//    out.push_back({point.x * scale.x, point.y * scale.y, point.z * scale.z});
//  }
//  return out;
//}

#endif
