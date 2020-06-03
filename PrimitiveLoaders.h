//
//  PrimitiveLoaders.h
//  ComponentTest
//
//  Created by Ben on 03/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef ComponentTest_PrimitiveLoaders_h
#define ComponentTest_PrimitiveLoaders_h
	
#include "Primitive.h"

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
