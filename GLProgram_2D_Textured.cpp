//
//  GLProgram_2D_Textured.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_2D_Textured.h"
#include "GLHelpers.h"

enum {
	_PROG_2D_TEXTURED_INDEX_OF_VERT_POSITION_ATTRIB,
	_PROG_2D_TEXTURED_INDEX_OF_TEXCOORD_ATTRIB
};

GLProgram_2D_Textured::GLProgram_2D_Textured() :
	GLProgram("2D_Textured_v.sh", "2D_Textured_f.sh")
{
	
}

void GLProgram_2D_Textured::setAttribLocations() {
	prog_setAttribLocation(programID, _PROG_2D_TEXTURED_INDEX_OF_VERT_POSITION_ATTRIB, "inVPos");
	prog_setAttribLocation(programID, _PROG_2D_TEXTURED_INDEX_OF_TEXCOORD_ATTRIB, "inTexCoord");
}
bool GLProgram_2D_Textured::getUniformLocations() {
	samplerUniformLoc = prog_uniformLocation(programID, "uTex");
	if (samplerUniformLoc == -1) return false;
	
	return true;
}

void GLProgram_2D_Textured::setBuffersForAttribs(unsigned int vert_vboID, unsigned int tex_vboID) {
	vboID_vertexPos = vert_vboID;
	vboID_texcoords = tex_vboID;
}

void GLProgram_2D_Textured::setTextureUnit(int texUnit) {
	prog_setUniformValue_Int(samplerUniformLoc, texUnit);
}

void GLProgram_2D_Textured::setUpAttribsForDrawing() {
	prog_setAttribToUseVBO(_PROG_2D_TEXTURED_INDEX_OF_VERT_POSITION_ATTRIB, vboID_vertexPos, 2, ATTRTYPE_FLOAT);
	prog_setAttribToUseVBO(_PROG_2D_TEXTURED_INDEX_OF_TEXCOORD_ATTRIB, vboID_texcoords, 2, ATTRTYPE_FLOAT);
}
