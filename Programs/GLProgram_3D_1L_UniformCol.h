//
//  GLProgram_NEWTEST_CI_IMM.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_1L_UniformCol_h
#define __GLProgram_3D_1L_UniformCol_h

#include "GLProgram.h"

class GLProgram_3D_1L_UniformCol : public GLProgram {
public:
	GLProgram_3D_1L_UniformCol();
	
	bool ok;
	
	class Attribs {
	public:
		enum {
			VertPos,    Normal,    Colour,
			UnitPos,    UnitRot
		};
	};
	
	class Uniforms {
	public:
		enum {
			MVPMtx,       ModelMtx,      NormalMtx,
			LightVec,     LightProperties,
			Colour,       ColToLightRatio,
			CamPos
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal,
			colour,
			unitPos,
			unitRot;
	} buffers;
	
};


#endif
