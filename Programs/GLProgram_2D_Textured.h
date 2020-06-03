//
//  GLProgram_2D_Textured.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GLPROGRAM_2D_TEXTURED_H
#define __GLPROGRAM_2D_TEXTURED_H

#include "GLProgram.h"

class GLProgram_2D_Textured : public GLProgram {
public:
	GLProgram_2D_Textured(const char *fragsh = "2D_Textured.fsh");
	
	bool ok;
	
	class Attribs {
	public:
		enum { VertPos, TexCoord };
	};
	
	class Uniforms {
	public:
		enum { Sampler };
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			texCoord;
	} buffers;
	
	// To use, bind the texture, and set the tex unit via
	// the uniform.
	
};

#endif