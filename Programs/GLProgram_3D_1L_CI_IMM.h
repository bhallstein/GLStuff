//
//  GLProgram_NEWTEST_CI_IMM.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_1L_CI_IMM_h
#define __GLProgram_3D_1L_CI_IMM_h

#include "GLProgram.h"

class GLProgram_3D_1L_CI_IMM : public GLProgram {
public:
	GLProgram_3D_1L_CI_IMM();
	
	bool ok;
	
	class Attribs {
	public:
		enum {
			VertPos,    Normal,    Colour,
			ModelPos,    ModelRot
		};
	};
	
	class Uniforms {
	public:
		enum {
			ModelMtx,     ViewMtx,      ProjMtx,    NormalMtx,
			LightVec,     LightProperties,          ColToLightRatio,
			CamPos
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal,
			colour,
			modelPos,
			quaternion;
	} buffers;
	
};


#endif
