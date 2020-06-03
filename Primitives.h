//
//  Primitives.h
//
//

#ifndef __Primitives_h
#define __Primitives_h

#include "CoordinateTypes.h"


struct Triangle {
	v3f vertices[3];
};
extern Triangle unitTriangle_Equil;	 // Side length = 1

struct Triangle2D {
	v2f vertices[3];
	Triangle2D(const Triangle &);
};


struct Rectangle {
	v3f vertices[6];
	Rectangle operator* (float);
	Rectangle operator* (const v2f &);
};
extern Rectangle unitSquare;

struct Rectangle2D {
	v2f vertices[6];
	Rectangle2D(const Rectangle &);
};


struct Cube {
	v3f vertices[36];
};
extern Cube unitCube_vert;
extern Cube unitCube_norm;


struct SquarePyramid {
	v3f vertices[18];
	v3f normals[18];
	
	SquarePyramid operator* (float);
	SquarePyramid operator* (const v2f &);
	SquarePyramid operator* (const v3f &);
	
private:
	struct __Init;
	static __Init *__init;
};
extern SquarePyramid unitSqPyramid;


#endif

