//
//  Renderer.cpp
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "Renderer_3D_1L_UniformCol.h"

#include <OpenGL/gl3.h>
#include "GLHelpers.h"
#include "GLProg.h"
#include "Camera.h"
#include "Lights.h"
#include "Primitives.h"
#include "FilePaths_CPP.h"


Renderer_3D_1L_UniformCol::Renderer_3D_1L_UniformCol() :
	prog(NULL),
	vao(0),
	colour{ 1.0, 0.75, 0.25 },
	colToLightRatio(0.75)
{
	
}

Renderer_3D_1L_UniformCol::~Renderer_3D_1L_UniformCol()
{
	if (prog) delete prog;
}

bool Renderer_3D_1L_UniformCol::setUp() {
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos = vbo_create();
	buffers.normal    = vbo_create();
	
	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_UniformColour.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_UniformColour.fsh");
	prog->attribs = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,  buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3, false }
	};
	prog->uniforms = {
		{ Uniforms::MVPMtx,          "mvpMtx"           },
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
	
	n_vertices = 36;
	
	vao_bind(0);
	return true;
}

void Renderer_3D_1L_UniformCol::render(Camera *cam, DirectionalLight *light, glm::mat4 &m_model) {
	prog_use(prog->programID);
	vao_bind(vao);
	
	// Uniforms
	
	// mvp matrices
	glm::mat4 m_view = glm::make_mat4(cam->getViewMatrix());
	glm::mat4 m_proj = glm::make_mat4(cam->getProjMatrix());
	glm::mat4 m_mvp = m_proj * m_view * m_model;
	glm::mat3 m_normal = glm::inverse(glm::transpose(glm::mat3(m_model)));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::MVPMtx), mptr(m_mvp));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::ModelMtx), mptr(m_model));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::NormalMtx), mptr(m_normal));
	
	// lights
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightVec), light->lightVector);
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightProperties), light->lightProperties);
	
	// colour
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::Colour), (float*) &colour);
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), cam->pos);
	
	glDrawArrays(GL_TRIANGLES, 0, n_vertices);
	
	vao_bind(0);
}

