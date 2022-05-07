//
//  Renderer.cpp
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "Renderer_3D_1L_Textured.h"
#include "GLProg.h"
#include "ObjImport.h"

#include <OpenGL/gl3.h>
#include "Camera.h"
#include "Lights.h"
#include "FilePaths_CPP.h"


Renderer_3D_1L_Textured::Renderer_3D_1L_Textured() :
	prog(NULL),
	vao(0),
	tex(0),
	n_elements(-1),
	colToLightRatio(0.75)
{
	// hi
}

Renderer_3D_1L_Textured::~Renderer_3D_1L_Textured()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.normal);
	vbo_delete(buffers.texcoord);
	vbo_delete(buffers.elements);
	vao_delete(vao);
	if (prog) delete prog;
		// NB: The texture is assumed to be managed externally,
		//     so is not automatically deleted.
}

bool Renderer_3D_1L_Textured::setUp() {
	vao = vao_create();
	vao_bind(vao);
	
	buffers.vertexPos   = vbo_create();
	buffers.normal      = vbo_create();
	buffers.texcoord    = vbo_create();
	buffers.elements    = vbo_create();
	
	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_Textured.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_Textured.fsh");
	prog->attribs = {
		{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,      buffers.normal,      "inVNormal",  ATTRTYPE_FLOAT, 3, false },
		{ Attribs::TexCoord,    buffers.texcoord,    "inTexCoord", ATTRTYPE_FLOAT, 2, false },
	};
	prog->uniforms = {
		{ Uniforms::MVPMtx,          "mvpMtx" },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::Sampler,         "uTex"             },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" },
		{ Uniforms::CamPos,          "uCamPosition"     },
	};
	
	vbo_bind(buffers.elements, VBOTYPE_ELEMENT_ARRAY);
	
	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}
	
	vao_bind(0);
	return true;
}
void Renderer_3D_1L_Textured::setObj(ObjFile *obj) {
	// VBOs
	int n_bytes = sizeof(v3) * obj->nVerticesImported;
	int n_bytes_uv = sizeof(v2) * obj->nVerticesImported;
	int n_bytes_elements = sizeof(int) * obj->nElements;
	
	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(n_bytes, obj->vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(n_bytes, obj->normals, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	vbo_bind(buffers.texcoord, VBOTYPE_ARRAY);
	vbo_upload(n_bytes_uv, obj->texcoords, VBOTYPE_ARRAY, VBOHINT_STATIC);
	
	vbo_bind(buffers.elements, VBOTYPE_ELEMENT_ARRAY);
	vbo_upload(n_bytes_elements, obj->elements, VBOTYPE_ELEMENT_ARRAY, VBOHINT_STATIC);
	
	n_elements = obj->nElements;
}
void Renderer_3D_1L_Textured::setTex(unsigned int _tex) {
	tex = _tex;
}

void Renderer_3D_1L_Textured::render(Camera *cam, DirectionalLight *light, glm::mat4 &m_model) {
	if (n_elements < 0) {
		printf("Renderer_3D_1L_Textured::render() called but n_elements is -1");
		return;
	}
	
	prog_use(prog->programID);
	vao_bind(vao);
	
	tx_bind(tex);
	
	// Uniforms
	glm::mat4 m_view   = glm::make_mat4(cam->getViewMatrix());
	glm::mat4 m_proj   = glm::make_mat4(cam->getProjMatrix());
	glm::mat4 m_mvp = m_proj * m_view * m_model;
	glm::mat3 m_normal = glm::inverse(glm::transpose(glm::mat3(m_model)));
	
	// mvp matrices
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::MVPMtx), mptr(m_mvp));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::ModelMtx), mptr(m_model));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::NormalMtx), mptr(m_normal));
	
	// tex
	prog_setUniformValue_Int(prog->uniformID(Uniforms::Sampler), 0);
	
	// lights
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightProperties), light->lightProperties);
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightVec), light->lightVector);
	
	// colour
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);
	
	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), cam->pos);
	
	glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
	

//	glDrawArrays(GL_TRIANGLES, 0, n_elements);
}

