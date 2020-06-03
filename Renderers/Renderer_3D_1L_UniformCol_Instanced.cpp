//
//  GLProgram_3D_1L_UniformCol.cpp
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "Renderer_3D_1L_UniformCol_Instanced.h"
#include "GLHelpers.h"
#include "GLProg.h"
#include "FilePaths_CPP.h"
#include "Primitives.h"
#include "Camera.h"
#include "Lights.h"
#include <OpengL/gl3.h>

Renderer_3D_1L_UniformCol_Instanced::Renderer_3D_1L_UniformCol_Instanced() :
	prog(NULL),
	vao(0),
	colour{ 1.0, 0.75, 0.25 },
	colToLightRatio(0.75)
{
	// hi
}

Renderer_3D_1L_UniformCol_Instanced::~Renderer_3D_1L_UniformCol_Instanced()
{
	if (prog) delete prog;
}

bool Renderer_3D_1L_UniformCol_Instanced::setUp() {
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos   = vbo_create();
	buffers.normal      = vbo_create();
	buffers.translation = vbo_create();
	buffers.quaternion  = vbo_create();
	
	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_UniformColour_Instanced.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_UniformColour.fsh");
	prog->attribs = {
		{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,      buffers.normal,      "inVNormal",  ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Quaternion,  buffers.quaternion,  "inQuaternion", ATTRTYPE_FLOAT, 4, true },
		{ Attribs::Translation, buffers.translation, "inTranslation", ATTRTYPE_FLOAT, 3, true },
	};
	prog->uniforms = {
		{ Uniforms::VPMtx,           "vpMtx"            },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos,          "uCamPosition"     },
		{ Uniforms::Colour,          "uColour"          },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" },
	};
	
	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}
	
	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*36, unitCube_vert.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*36, unitCube_norm.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	v3 t[] = { {1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0 } };
	vbo_bind(buffers.translation, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*2, t, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	glm::quat q = glm::angleAxis(RAD(33), glm::vec3(0.0, 1.0, 0.0));
	v4 qv = { q[0], q[1], q[2], q[3] };
	v4 qs[] = { qv, qv };
	vbo_bind(buffers.quaternion, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v4)*2, qs, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	n_vertices_per_model = 36;
	n_models = 2;
	
	vao_bind(0);
	return true;
}

void Renderer_3D_1L_UniformCol_Instanced::render(Camera *cam, DirectionalLight *light, glm::mat3 &m_model) {
	prog_use(prog->programID);
	vao_bind(vao);
	
	// Uniforms
	
	// mvp matrices
	glm::mat4 m_view = glm::make_mat4(cam->getViewMatrix());
	glm::mat4 m_proj = glm::make_mat4(cam->getProjMatrix());
	glm::mat4 m_vp   = m_proj * m_view;
	glm::mat3 m_normal = glm::inverse(glm::transpose(m_model));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::VPMtx), mptr(m_vp));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::ModelMtx), mptr(m_model));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::NormalMtx), mptr(m_normal));
	
	// lights
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightVec), light->lightVector);
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightProperties), light->lightProperties);
	
	// colour
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::Colour), (float*) &colour);
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);
	
	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), cam->pos);
	
	
	glDrawArraysInstanced(GL_TRIANGLES, 0, n_vertices_per_model, n_models);
	
	vao_bind(0);
}

