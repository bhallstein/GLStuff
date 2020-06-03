//
//  GLProgram_2D_Textured.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GLPROGRAM_2D_Refl_Mapped_H
#define __GLPROGRAM_2D_Refl_Mapped_H

#include "GLProgram.h"

class GLProgram_3D_1L_RM : public GLProgram {
public:
	GLProgram_3D_1L_RM();
	
	bool ok;
	
	class Attribs {
	public:
		enum { VertPos, Normal };
	};
	
	class Uniforms {
	public:
		enum {
			MVPMtx,       ModelMtx,      NormalMtx,
			LightVec,     LightProperties,
			Ratio,
			CamPos,
			
			CubeMapTexUnit
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal;
	} buffers;
	
};

#endif

