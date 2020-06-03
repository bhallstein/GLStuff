//
//  GLProgram_3D_1L_UniformCol.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_1L_UniformCol_IMM_h
#define __GLProgram_3D_1L_UniformCol_IMM_h

#include "GLProgram.h"

class GLProgram_3D_1L_UniformCol_IMM : public GLProgram {
public:
	GLProgram_3D_1L_UniformCol_IMM();
	
	bool ok;
	
	class Attribs {
	public:
		enum {
			VertPos,    Normal,
			Translation,
			Quaternion
		};
	};
	
	class Uniforms {
	public:
		enum {
			ModelMtx,     ViewMtx,           ProjMtx,       NormalMtx,         
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
			translation,
			quaternion;
	} buffers;
	
};


#endif
