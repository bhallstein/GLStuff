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
	GLProgram_2D_Textured();
	
	void setBuffersForAttribs(unsigned int vert_vboID, unsigned int tex_vboID);
	void setUpAttribsForDrawing();
	void setTextureUnit(int);
	
protected:
	void setAttribLocations();
	bool getUniformLocations();
	
	int samplerUniformLoc;
	int vboID_vertexPos;
	int vboID_texcoords;
};

#endif