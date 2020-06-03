//
//  GLProgram_2D_ColourIndexed.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_3D_1L_RM.h"
#include "GLHelpers.h"


GLProgram_3D_1L_RM::GLProgram_3D_1L_RM() :
	GLProgram("3D_RM.vsh", "3D_RM.fsh")
{
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos  = vbo_create();
	buffers.normal     = vbo_create();
	
	std::vector<AttribInfo> attribsDefinition = {
		{ Attribs::VertPos,  buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,   buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3, false }
	};
	std::vector<UniformInfo> uniformsDefinition = {
		{ Uniforms::MVPMtx,          0, "mvpMtx"            },
		{ Uniforms::ModelMtx,        0, "modelMtx"          },
		{ Uniforms::NormalMtx,       0, "normalMtx"         },
		{ Uniforms::LightVec,        0, "uLightVector"      },
		{ Uniforms::LightProperties, 0, "uLightProperties"  },
		{ Uniforms::CamPos,          0, "uCamPosition"      },
		{ Uniforms::Ratio,           0, "uReflToLightRatio" },
		{ Uniforms::CubeMapTexUnit,  0, "uCubeMapTexUnit"   }
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

