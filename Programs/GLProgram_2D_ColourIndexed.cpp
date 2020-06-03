//
//  GLProgram_2D_Textured.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_2D_ColourIndexed.h"
#include "GLHelpers.h"

GLProgram_2D_ColourIndexed::GLProgram_2D_ColourIndexed() :
	GLProgram("2D_ColourIndexed.vsh", "2D_ColourIndexed.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos = vbo_create();
	buffers.colour    = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos,  buffers.vertexPos, "inVPos",   ATTRTYPE_FLOAT, 2 },
		{ Attribs::Colour,   buffers.colour,    "inColour", ATTRTYPE_FLOAT, 3 }
	};
	
	bool compiled = compile(attribsDefinition, { });
	if (!compiled)
		printf("Failed to compile program\n");
	
	bool validated = validate();
	if (!validated)
		printf("Failed to validate program\n");
	
	ok = compiled && validated;
	
	vao_bind(0);
}
