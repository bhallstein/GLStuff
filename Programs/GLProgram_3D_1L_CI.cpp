//
//  GLProgram_2D_ColourIndexed.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_CI.h"
#include "GLHelpers.h"

GLProgram_3D_1L_CI::GLProgram_3D_1L_CI() :
	GLProgram("3D_ColourIndexed.vsh", "3D_ColourIndexed.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos = vbo_create();
	buffers.normal = vbo_create();
	buffers.colour = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3 },
		{ Attribs::Normal,  buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3 },
		{ Attribs::Colour,  buffers.colour,    "inColour",  ATTRTYPE_FLOAT, 3 }
	};
	std::vector<UniformInfo> uniformsDefinition = {
		{ Uniforms::MVPMtx, "mvpMtx" },
		{ Uniforms::ModelMtx, "modelMtx" },
		{ Uniforms::NormalMtx, "normalMtx" },
		{ Uniforms::LightVec, "uLightVector" },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos, "uCamPosition" },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" }
	};
	
	bool compiled = compile(attribsDefinition, uniformsDefinition);
	if (!compiled)
		printf("Failed to compile program\n");
	
	bool validated = validate();
	if (!validated)
		printf("Failed to validate program\n");
	
	ok = compiled && validated;
	
	vao_bind(0);
	
}

