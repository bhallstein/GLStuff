//
//  GLProgram_NEWTEST_CI_IMM.cpp
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_Textured.h"

GLProgram_3D_1L_Textured::GLProgram_3D_1L_Textured() :
	GLProgram("3D_Textured.vsh", "3D_Textured.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos   = vbo_create();
	buffers.normal      = vbo_create();
	buffers.texcoord    = vbo_create();
	buffers.elements    = vbo_create();
	buffers.translation = vbo_create();
	buffers.quaternion  = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,      buffers.normal,      "inVNormal",  ATTRTYPE_FLOAT, 3, false },
		{ Attribs::TexCoord,    buffers.texcoord,    "inTexCoord", ATTRTYPE_FLOAT, 2, false },
		{ Attribs::Translation, buffers.translation, "inModelPos", ATTRTYPE_FLOAT, 3, true },
		{ Attribs::Quaternion,  buffers.quaternion,  "inModelRot", ATTRTYPE_FLOAT, 4, true },
	};
	std::vector<UniformInfo> uniformsDefinition = {
		{ Uniforms::ModelMtx,        0, "modelMtx"         },
		{ Uniforms::ViewMtx,         0, "viewMtx"          },
		{ Uniforms::ProjMtx,         0, "projMtx"          },
		{ Uniforms::NormalMtx,       0, "normalMtx"        },
		{ Uniforms::Sampler,         0, "uTex"             },
		{ Uniforms::LightVec,        0, "uLightVector"     },
		{ Uniforms::LightProperties, 0, "uLightProperties" },
		{ Uniforms::ColToLightRatio, 0, "uColToLightRatio" },
		{ Uniforms::CamPos,          0, "uCamPosition"     },
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

