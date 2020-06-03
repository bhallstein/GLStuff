//
//  GLProgram_3D_1L_UniformCol.cpp
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_UniformCol.h"

GLProgram_3D_1L_UniformCol::GLProgram_3D_1L_UniformCol() :
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
		{ Uniforms::MVPMtx,          "mvpMtx"           },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos,          "uCamPosition"     },
		{ Uniforms::Colour,          "uColour"          },
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

