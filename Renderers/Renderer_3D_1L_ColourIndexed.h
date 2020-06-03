//
//  Renderer-BG.h
//  IsometricOGLGame
//
//  Created by Ben on 09/07/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GLStuff_Renderer_3D_1L_ColourIndexed
#define __GLStuff_Renderer_3D_1L_ColourIndexed

#include "CoordinateTypes.h"

class GLProg;
class Camera;
struct DirectionalLight;

#include "glm_include.h"


class Renderer_3D_1L_ColourIndexed {
public:
	Renderer_3D_1L_ColourIndexed();
	~Renderer_3D_1L_ColourIndexed();
	
	bool setUp();
	void render(Camera *cam, DirectionalLight *light, glm::mat4 &m_model);
	
	void setRatio(float);
	
	struct Attribs {
		enum { VertPos,   Normal,    Colour };
	};
	
	struct Uniforms {
		enum {
			MVPMtx,          ModelMtx,      NormalMtx,
			LightVec,        LightProperties,
			ColToLightRatio,
			CamPos
		};
	};
	
	struct {
		unsigned int vertexPos, normal, colour;
	} buffers;
	
	int n_vertices;
	
private:
	GLProg *prog;
	unsigned int vao;
	
	float colToLightRatio;
	
};

#endif

