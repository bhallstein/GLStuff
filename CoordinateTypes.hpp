#ifndef CoordinateTypes_h
#define CoordinateTypes_h

struct v2  { float x, y; };
struct v3  { float x, y, z; };
struct v4  { float x, y, z, t; };

struct v2i { int x, y; };
struct v3i { int x, y, z; };

struct col3 { float r, g, b; };
struct col4 { float r, g, b, a; };


// equality testing funcs

inline bool eq(const v2 &a, const v2 &b) {
	return a.x == b.x && a.y == b.y;
}

inline bool eq(const v2i &a, const v2i &b) {
	return a.x == b.x && a.y == b.y;
}

inline bool eq(const v3 &a, const v3 &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool eq(const v3i &a, const v3i &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

inline bool eq(const v4 &a, const v4 &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z && a.t == b.t;
}

inline bool eq(const col3 &a, const col3 &b) {
	return a.r == b.r && a.g == b.g && a.b == b.b;
}

inline bool eq(const col4 &a, const col4 &b) {
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

#endif
