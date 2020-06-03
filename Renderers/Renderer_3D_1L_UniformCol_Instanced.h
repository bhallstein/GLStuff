//
//  GLProgram_3D_1L_UniformCol.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __Renderer_3D_1L_UniformCol_Instanced
#define __Renderer_3D_1L_UniformCol_Instanced

#include "glm_include.h"
#include "CoordinateTypes.h"

class GLProg;
class Camera;
class DirectionalLight;


class Renderer_3D_1L_UniformCol_Instanced {
public:
	Renderer_3D_1L_UniformCol_Instanced();
	~Renderer_3D_1L_UniformCol_Instanced();
	
	bool setUp();
	void render(Camera*, DirectionalLight*, glm::mat3 &mtx_model);
	
	struct Attribs {
		enum { VertPos, Normal, Quaternion, Translation };
	};
	
	struct Uniforms {
		enum {
			VPMtx,    ModelMtx,   NormalMtx,
			LightVec, LightProperties,
			Colour,   ColToLightRatio,
			CamPos
		};
	};
	
	struct {
		unsigned int vertexPos, normal, quaternion, translation;
	} buffers;
	
	int n_vertices_per_model;
	int n_models;
	
private:
	GLProg *prog;
	unsigned int vao;
	
	float colToLightRatio;
	v3 colour;
};


#endif
