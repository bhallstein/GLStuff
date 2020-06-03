//
//  GLProgram_NEWTEST_CI_IMM.cpp
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_CI_IMM.h"

GLProgram_3D_1L_CI_IMM::GLProgram_3D_1L_CI_IMM() :
	GLProgram("3D_CI_IMM.vsh", "3D_CI_IMM.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos   = vbo_create();
	buffers.normal      = vbo_create();
	buffers.colour      = vbo_create();
	buffers.translation = vbo_create();
	buffers.quaternion  = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,      buffers.normal,      "inVNormal",  ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Colour,      buffers.colour,      "inColour",   ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Translation, buffers.translation, "inModelPos", ATTRTYPE_FLOAT, 3, true },
		{ Attribs::Quaternion,  buffers.quaternion,  "inModelRot", ATTRTYPE_FLOAT, 4, true },
	};
	std::vector<UniformInfo> uniformsDefinition = {
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::ViewMtx,         "viewMtx"          },
		{ Uniforms::ProjMtx,         "projMtx"          },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos,          "uCamPosition"     },
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

