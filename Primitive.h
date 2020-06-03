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
	Primitive(int _nVertices) : nVertices(_nVertices)
	{
		vertices = new v3f[nVertices];
		normals = new v3f[nVertices];
	}
	~Primitive()
	{
		delete [] vertices;
		delete [] normals;
	}
	int nVertices;
	v3f *vertices;
	v3f *normals;
};

struct Primitive2D {
	Primitive2D(int _nVertices) : nVertices(_nVertices)
	{
		vertices = new v2f[nVertices];
	}
	~Primitive2D()
	{
		delete [] vertices;
	}
	int nVertices;
	v2f *vertices;
};

#endif