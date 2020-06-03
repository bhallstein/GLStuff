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

class GLProgram_2D_ColourIndexed : public GLProgram {
public:
	GLProgram_2D_ColourIndexed();
	
	bool ok;
	
	class Attribs {
	public:
		enum { VertPos, Colour };
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			colour;
	} buffers;
	
};

#endif
