//
//  PrimitivePresets.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __PRIMITIVE_PRESETS_H
#define __PRIMITIVE_PRESETS_H

#include "CoordinateTypes.h"

struct Primitive {
	Primitive(int _nVertices);
	~Primitive();
	int nVertices;
	v3f *vertices;
	v3f *normals;
};

Primitive pr_loadTriangle_Equil(float radius);		// centre at O
Primitive pr_loadTriangle_Isosc(float w, float h);	// base at O

//void pr_loadTriangle_Scale();

Primitive pr_loadRectangle(float w, float h);		// centre at O

Primitive pr_loadCuboid(float w, float h, float d);	// centre at O

Primitive pr_loadSquarePyramid(float w, float h);	// base centred at O
//Primitive pr_loadTriPyramid(float baseRadius, float h);

#endif