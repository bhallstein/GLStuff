//
//  GLProgram_NEWTEST_CI_IMM.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_1L_Textured_h
#define __GLProgram_3D_1L_Textured_h

#include "GLProgram.h"

class GLProgram_3D_1L_Textured : public GLProgram {
public:
	GLProgram_3D_1L_Textured();
	
	bool ok;
	
	class Attribs {
	public:
		enum {
			VertPos,  Normal,  TexCoord, Elements,
			Translation,
			Quaternion
		};
	};
	
	class Uniforms {
	public:
		enum {
			ModelMtx,  ViewMtx,  ProjMtx,  NormalMtx,
			Sampler,
			LightVec,  LightProperties,  ColToLightRatio,
			CamPos
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal,
			elements,
		    texcoord,
			translation,
			quaternion;
	} buffers;
	
};


#endif
