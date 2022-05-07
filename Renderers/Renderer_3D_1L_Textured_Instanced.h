#ifndef __GLStuff_Renderer_3D_1L_Textured_Instanced
#define __GLStuff_Renderer_3D_1L_Textured_Instanced

#include <stdio.h>
#include "glm_include.h"

class GLProg;
class Camera;
class DirectionalLight;
class ObjFile;


class Renderer_3D_1L_Textured_Instanced {
public:
	Renderer_3D_1L_Textured_Instanced();
	~Renderer_3D_1L_Textured_Instanced();

	bool setUp();
	void setObj(ObjFile*);
	void setTex(unsigned int);
	// set sampler?
	void render(Camera, DirectionalLight*, glm::mat3 &mtx_model);

	class Attribs {
	public:
		enum { VertPos, Normal, TexCoord, Elements, Quaternion, Translation };
	};

	class Uniforms {
	public:
		enum {
			VPMtx, ModelMtx,  NormalMtx,
			Sampler,
			LightVec,  LightProperties,  ColToLightRatio,
			CamPos
		};
	};

	struct {
		unsigned int vertexPos, normal, texcoord, elements, quaternion, translation;
	} buffers;

	int n_elements_per_model;
	int n_models;

private:
	GLProg *prog;
	unsigned int vao;

	float colToLightRatio;
	unsigned int tex;

};

#endif

