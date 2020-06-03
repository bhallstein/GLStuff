//
//  GLProgram_3D_1L_UniformCol.cpp
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_UniformCol-Def.h"

GLProgram_3D_1L_UniformCol_Def::GLProgram_3D_1L_UniformCol_Def() :
	GLProgram("3D_UniformColour.vsh", "3D_UniformColour.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos = vbo_create();
	buffers.normal    = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,  buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3, false }
	};
	std::vector<UniformInfo> uniformsDefinition = {
		{ Uniforms::MVPMtx,    0, "mvpMtx"       },
		{ Uniforms::ModelMtx,  0, "modelMtx"     },
		{ Uniforms::NormalMtx, 0, "normalMtx"    },
		{ Uniforms::CamPos,    0, "uCamPosition" },
	};
	std::vector<ColorAttachmentInfo> attachmentsDefinition = {
		{ Attachments::Normal, "fragOutNormal" }
	};
	
	bool compiled = compile(attribsDefinition, uniformsDefinition, attachmentsDefinition);
	if (!compiled)
		printf("Failed to compile program\n");
	
	bool validated = validate();
	if (!validated)
		printf("Failed to validate program\n");
	
	ok = compiled && validated;
	
	vao_bind(0);
}

