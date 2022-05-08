#include "Primitives.hpp"
#include <cstdio>
#include <cmath>

// Triangle
// --------------------------------------

Triangle unitTriangle_Equil = {
	 0.0,  0.433,  0.0,
	-0.5, -0.433,  0.0,
	 0.5, -0.433,  0.0
};

Triangle2D::Triangle2D(const Triangle &t) {
	for (int i=0; i < 3; ++i) {
		const v3 &v = t.vertices[i];
		vertices[i] = { v.x, v.y };
	}
}


// Rectangle
// --------------------------------------

Rectangle unitSquare = {
	-0.5,   0.5,  0.0,
	-0.5,  -0.5,  0.0,
	 0.5,  -0.5,  0.0,
	
	-0.5,   0.5,  0.0,
	 0.5,  -0.5,  0.0,
	 0.5,   0.5,  0.0,
};

Rectangle unitSquare_texcoords {
	0.0,  1.0,  0.0,
	0.0,  0.0,  0.0,
	1.0,  0.0,  0.0,
	
	0.0,  1.0,  0.0,
	1.0,  0.0,  0.0,
	1.0,  1.0,  0.0,
};

Rectangle2D unitSquare_2D = unitSquare;
Rectangle2D unitSquare_texcoords_2D = unitSquare_texcoords;

Rectangle Rectangle::operator*(float x) {
	Rectangle r;
	for (int i=0; i < 6; ++i) {
		const v3 &v = vertices[i];
		r.vertices[i] = { v.x*x, v.y*x, v.z };
	}
	return r;
}

Rectangle Rectangle::operator*(const v2 &x) {
	Rectangle r;
	for (int i=0; i < 6; ++i) {
		const v3 &v = vertices[i];
		r.vertices[i] = { x.x*v.x, x.y*v.y, v.z };
	}
	return r;
}


Rectangle2D::Rectangle2D(const Rectangle &r)
{
	for (int i=0; i < 6; ++i) {
		const v3 &v = r.vertices[i];
		vertices[i] = { v.x, v.y };
	}
}

Rectangle2D Rectangle2D::operator+(const v2 &r) {
	Rectangle2D out;
	for (int i=0; i < 6; ++i) {
		const v2 &p = vertices[i];
		out.vertices[i] = { p.x + r.x, p.y + r.y };
	}
	return out;
}
Rectangle2D Rectangle2D::operator+(float x) {
	Rectangle2D out;
	for (int i=0; i < 6; ++i) {
		const v2 &p = vertices[i];
		out.vertices[i] = { p.x + x, p.y + x };
	}
	return out;
}

Rectangle2D Rectangle2D::operator*(const v2 &r) {
	Rectangle2D out;
	for (int i=0; i < 6; ++i) {
		const v2 &p = vertices[i];
		out.vertices[i] = { p.x * r.x, p.y * r.y };
	}
	return out;
}



// Cube
// --------------------------------------

Cube unitCube_vert = {
	-0.5, -0.5,  -0.5,
	-0.5,  0.5,  -0.5,
	0.5,  0.5,  -0.5,
	-0.5, -0.5,  -0.5,
	0.5,  0.5,  -0.5,
	0.5, -0.5,  -0.5,
	
	0.5, -0.5,  0.5,
	0.5,  0.5,  0.5,
	-0.5,  0.5,  0.5,
	0.5, -0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5, -0.5,  0.5,
	
	-0.5,  0.5, -0.5,
	-0.5,  0.5,  0.5,
	0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	0.5,  0.5,  0.5,
	0.5,  0.5, -0.5,
	
	-0.5, -0.5,  0.5,
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	-0.5, -0.5,  0.5,
	0.5, -0.5, -0.5,
	0.5, -0.5,  0.5,
	
	-0.5, -0.5,  0.5,
	-0.5,  0.5,  0.5,
	-0.5,  0.5, -0.5,
	-0.5, -0.5,  0.5,
	-0.5,  0.5, -0.5,
	-0.5, -0.5, -0.5,
	
	0.5, -0.5, -0.5,
	0.5,  0.5, -0.5,
	0.5,  0.5,  0.5,
	0.5, -0.5, -0.5,
	0.5,  0.5,  0.5,
	0.5, -0.5,  0.5,
};
Cube unitCube_norm = {
	// Front face
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	0.0, 0.0, -1.0,
	
	// Back face
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	0.0, 0.0, 1.0,
	
	// Top face
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	0.0, 1.0, 0.0,
	
	// Bottom face
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	0.0, -1.0, 0.0,
	
	// Left face
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	-1.0, 0.0, 0.0,
	
	// Right face
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
	1.0, 0.0, 0.0,
};


// Square Pyramid
// --------------------------------------


#define __SQPYR_BASE_LEFT_NEAR  { -0.5, 0.0, -0.5 }
#define __SQPYR_BASE_LEFT_FAR   { -0.5, 0.0,  0.5 }
#define __SQPYR_BASE_RIGHT_NEAR {  0.5, 0.0, -0.5 }
#define __SQPYR_BASE_RIGHT_FAR  {  0.5, 0.0,  0.5 }
#define __SQPYR_APEX            {  0.0, 1.0,  0.0 }

SquarePyramid unitSqPyramid = {
	// Vertices
	{
		__SQPYR_BASE_LEFT_FAR,		// Base
		__SQPYR_BASE_LEFT_NEAR,
		__SQPYR_BASE_RIGHT_NEAR,
		__SQPYR_BASE_LEFT_FAR,
		__SQPYR_BASE_RIGHT_NEAR,
		__SQPYR_BASE_RIGHT_FAR,

		__SQPYR_BASE_LEFT_NEAR,		// Near triangle
		__SQPYR_APEX,
		__SQPYR_BASE_RIGHT_NEAR,

		__SQPYR_BASE_RIGHT_FAR,		// Far triangle
		__SQPYR_APEX,
		__SQPYR_BASE_LEFT_FAR,

		__SQPYR_BASE_LEFT_FAR,		// Left triangle
		__SQPYR_APEX,
		__SQPYR_BASE_LEFT_NEAR,

		__SQPYR_BASE_RIGHT_NEAR,	// Right triangle
		__SQPYR_APEX,
		__SQPYR_BASE_RIGHT_FAR
	}
};

void __sqpyr_set_normals(float w, float h, SquarePyramid &p) {
	// t is a vector parallel to the normal. It is simply (-w/2, h).
	// n is (nHoriz, nVert) - t normalized.
	float modT = sqrtf(w*w + h*h);
	float nHoriz = w / modT;
	float nVert  = h / modT;
	
	v3 normals[] = {
		0.0, -1.0, 0.0,		// Base
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		0.0, -1.0, 0.0,
		
		0.0, nVert, -nHoriz,		// Near triangle
		0.0, nVert, -nHoriz,
		0.0, nVert, -nHoriz,
		
		0.0, nVert, nHoriz,		// Far triangle
		0.0, nVert, nHoriz,
		0.0, nVert, nHoriz,
		
		-nHoriz, nVert, 0.0,	// Left triangle
		-nHoriz, nVert, 0.0,	
		-nHoriz, nVert, 0.0,	
		
		nHoriz, nVert, 0.0,		// Right triangle
		nHoriz, nVert, 0.0,
		nHoriz, nVert, 0.0,
	};
	
	for (int i=0; i < 18; ++i)
		p.normals[i] = normals[i];
}

struct SquarePyramid::__Init {
	__Init() {
		__sqpyr_set_normals(1.0, 1.0, unitSqPyramid);
	}
};
SquarePyramid::__Init *SquarePyramid::__init = new SquarePyramid::__Init;

SquarePyramid SquarePyramid::operator*(float x) {
	SquarePyramid p;
	for (int i=0; i < 18; ++i) {
		const v3 &v = vertices[i];
		p.vertices[i] = { v.x*x, v.y*x, v.z*x };
		p.normals[i] = normals[i];
	}
	return p;
}

SquarePyramid SquarePyramid::operator*(const v2 &x) {
	SquarePyramid p;
	
	for (int i=0; i < 18; ++i) {
		const v3 &v = vertices[i];
		p.vertices[i] = { v.x*x.x, v.y*x.y, v.z*x.x };
	}
	__sqpyr_set_normals(p.vertices[2].x * 2.0,
						p.vertices[7].y,
						p);
	return p;
}


// Jewel
// --------------------------------------

Jewel unitJewel;

void __jewel_set_normals(float width, float height, Jewel &jwl) {
	SquarePyramid pyr = unitSqPyramid * (v2){ width, height };
	
	for (int i=0; i < 18; ++i)
		jwl.normals[i] = pyr.normals[i];
	
	for (int i=0; i < 12; ++i) {
		const v3 &n = pyr.normals[i * 6];
		jwl.normals[i+18] = (v3) { n.x, -n.y, n.z };
	}
}

struct Jewel::__Init {
	__Init() {
		for (int i=0; i < 18; ++i)
			unitJewel.vertices[i] = unitSqPyramid.vertices[i];
		
		for (int i=0; i < 12; ++i) {
			const v3 &v = unitSqPyramid.vertices[i+6];
			unitJewel.vertices[i+18] = (v3) { v.x, -v.y, v.z };
		}

		__jewel_set_normals(1.0, 1.0, unitJewel);
	}
};
Jewel::__Init *Jewel::__init = new Jewel::__Init;

Jewel Jewel::operator*(float x) {
	Jewel p;
	for (int i=0; i < 30; ++i) {
		const v3 &v = vertices[i];
		p.vertices[i] = { v.x*x, v.y*x, v.z*x };
		p.normals[i] = normals[i];
	}
	return p;
}

Jewel Jewel::operator*(const v2 &x) {
	Jewel p;
	for (int i=0; i < 30; ++i) {
		const v3 &v = vertices[i];
		p.vertices[i] = { v.x*x.x, v.y*x.y, v.z*x.x };
	}
	
	__jewel_set_normals(p.vertices[2].x * 2.0,
						p.vertices[7].y,
						p);
	
	return p;
}
