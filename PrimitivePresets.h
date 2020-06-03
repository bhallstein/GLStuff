//
//  PrimitivePresets.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __PRIMITIVE_PRESETS_H
#define __PRIMITIVE_PRESETS_H

class v3f;
class v2f;

struct Primitive {
	Primitive(int _nVertices);
	~Primitive();
	int nVertices;
	v3f *vertices;
	v3f *normals;
};

struct Primitive2D {
	Primitive2D(int _nVertices);
	~Primitive2D();
	int nVertices;
	v2f *vertices;
};

Primitive pr_loadTriangle_Equil(float radius);		// centre at O,
Primitive pr_loadTriangle_Isosc(float w, float h);	// base at O

Primitive2D pr_loadTriangle_Equil_2D(float radius);		// centre at O,
Primitive2D pr_loadTriangle_Isosc_2D(float w, float h);	// base at O

Primitive   pr_loadRectangle(float w, float h);	// on x-y plane,
Primitive2D pr_loadRectangle_2D(float w, float h);	// centred on 0

Primitive pr_loadCuboid(float w, float h, float d);	// centre at O

Primitive pr_loadSquarePyramid(float w, float h);	// base centred at O
//Primitive pr_loadTriPyramid(float baseRadius, float h);

#endif