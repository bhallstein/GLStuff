//
//  GLProgram_2D_Textured.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_ColourIndexed_h
#define __GLProgram_3D_ColourIndexed_h

#include "GLProgram.h"

class GLProgram_3D_1L_CI : public GLProgram {
public:
	GLProgram_3D_1L_CI();
	
	bool ok;
	
	class Attribs {
	public:
		enum {
			VertPos,   Normal,    Colour
		};
	};
	
	class Uniforms {
	public:
		enum {
			MVPMtx,       ModelMtx,      NormalMtx,
			LightVec,     LightProperties,
			ColToLightRatio,
			CamPos
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal,
			colour;
	} buffers;
	
};

#endif