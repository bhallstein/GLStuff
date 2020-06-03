//
//  FloorRenderer.h
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLStuff_Renderer_3D_1L_UniformCol
#define __GLStuff_Renderer_3D_1L_UniformCol

#include "TightlyPackedArray.h"
#include "CoordinateTypes.h"
#include "glm_include.h"

class GLProg;
class Camera;
class DirectionalLight;


class Renderer_3D_1L_UniformCol {
public:
	Renderer_3D_1L_UniformCol();
	~Renderer_3D_1L_UniformCol();
	
	bool setUp();
	void render(Camera*, DirectionalLight*, glm::mat4 &mtx_model);
	
	struct Attribs {
		enum {  VertPos, Normal  };
	};
	
	struct Uniforms {
		enum {
			MVPMtx,       ModelMtx,      NormalMtx,
			LightVec,     LightProperties,
			Colour,       ColToLightRatio,
			CamPos
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int vertexPos, normal;
	} buffers;
	
	int n_vertices;
	
private:
	GLProg *prog;
	
	float colToLightRatio;
	v3 colour;
	
};


#endif
