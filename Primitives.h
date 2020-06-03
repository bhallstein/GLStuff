//
//  Primitives.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 29/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __PRIMITIVES_H
#define __PRIMITIVES_H

#include "CoordinateTypes.h"

class Primitive {
public:
	bool updated;
};


class PrimitiveWith2DVertices {
public:
	v2f *vertices;
};

class PrimitiveWith3DVertices {
public:
	v3f *vertices;
};

class PrimitiveWithTexcoords {
public:
	v2f *texcoords;
};

class PrimitiveWithColours {
public:
	col3f *colours;
};


#endif
