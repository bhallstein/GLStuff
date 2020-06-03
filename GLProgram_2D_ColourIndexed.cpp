//
//  GLProgram_2D_Textured.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_2D_ColourIndexed.h"
#include "GLHelpers.h"

enum {
	_PROG_2D_COLOURINDEXED_INDEX_OF_VERT_POSITION_ATTRIB,
	_PROG_2D_COLOURINDEXED_INDEX_OF_COLOUR_ATTRIB
};

GLProgram_2D_ColourIndexed::GLProgram_2D_ColourIndexed() :
	GLProgram("2D_ColourIndexed_v.sh", "2D_ColourIndexed_f.sh")
{
	
}

void GLProgram_2D_ColourIndexed::setAttribLocations() {
	prog_setAttribLocation(programID, _PROG_2D_COLOURINDEXED_INDEX_OF_VERT_POSITION_ATTRIB, "inVPos");
	prog_setAttribLocation(programID, _PROG_2D_COLOURINDEXED_INDEX_OF_COLOUR_ATTRIB, "inColour");
}
bool GLProgram_2D_ColourIndexed::getUniformLocations() {
	return true;
}

void GLProgram_2D_ColourIndexed::setBuffersForAttribs(unsigned int vert_vboID, unsigned int col_vboID) {
	vboID_vertexPos = vert_vboID;
	vboID_colours = col_vboID;
}

void GLProgram_2D_ColourIndexed::setUpAttribsForDrawing() {
	prog_setAttribToUseVBO(_PROG_2D_COLOURINDEXED_INDEX_OF_VERT_POSITION_ATTRIB, vboID_vertexPos, 2, ATTRTYPE_FLOAT);
	prog_setAttribToUseVBO(_PROG_2D_COLOURINDEXED_INDEX_OF_COLOUR_ATTRIB, vboID_colours, 3, ATTRTYPE_FLOAT);
}
