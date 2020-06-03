//
//  UnitRenderer_textured.h
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLStuff_Renderer_3D_1L_Textured
#define __GLStuff_Renderer_3D_1L_Textured

#include <stdio.h>
#include "glm_include.h"

class GLProg;
class Camera;
class DirectionalLight;
class ObjFile;


class Renderer_3D_1L_Textured {
public:
	Renderer_3D_1L_Textured();
	~Renderer_3D_1L_Textured();
	
	bool setUp();
	void setObj(ObjFile*);
	void setTex(unsigned int);
	// set sampler?
	void render(Camera*, DirectionalLight*, glm::mat4 &mtx_model);
	
	class Attribs {
	public:
		enum {
			VertPos, Normal, TexCoord, Elements,
		};
	};
	
	class Uniforms {
	public:
		enum {
			MVPMtx, ModelMtx,  NormalMtx,
			Sampler,
			LightVec,  LightProperties,  ColToLightRatio,
			CamPos
		};
	};
	
	struct {
		unsigned int vertexPos, normal, texcoord, elements;
	} buffers;
	
	int n_elements;
	
private:
	GLProg *prog;
	unsigned int vao;
	
	float colToLightRatio;
	unsigned int tex;
	
};

#endif

