#ifndef Primitives_h
#define Primitives_h

#include "CoordinateTypes.hpp"


struct Triangle {
	v3 vertices[3];
};
extern Triangle unitTriangle_Equil;	 // Side length = 1

struct Triangle2D {
	v2 vertices[3];
	Triangle2D(const Triangle &);
};


struct Rectangle {
	v3 vertices[6];
	Rectangle operator* (float);
	Rectangle operator* (const v2 &);
};
extern Rectangle unitSquare;
extern Rectangle unitSquare_texcoords;

struct Rectangle2D {
	v2 vertices[6];
	Rectangle2D() { };
	Rectangle2D(const Rectangle &);
	Rectangle2D operator+ (const v2 &);
	Rectangle2D operator+ (float);
	Rectangle2D operator* (const v2 &);
};
extern Rectangle2D unitSquare_2D;
extern Rectangle2D unitSquare_texcoords_2D;


struct Cube {
	v3 vertices[36];
};
extern Cube unitCube_vert;
extern Cube unitCube_norm;


struct SquarePyramid {
	v3 vertices[18];
	v3 normals[18];
	
	SquarePyramid operator* (float);
	SquarePyramid operator* (const v2 &);
	
private:
	struct __Init;
	static __Init *__init;
};
extern SquarePyramid unitSqPyramid;


struct Jewel {
	v3 vertices[30];
	v3 normals[30];
	
	Jewel operator* (float);
	Jewel operator* (const v2 &);
	
private:
	struct __Init;
	static __Init *__init;
};
extern Jewel unitJewel;
	// The unit jewel has a width and depth of 1 and a height of 1

#endif

