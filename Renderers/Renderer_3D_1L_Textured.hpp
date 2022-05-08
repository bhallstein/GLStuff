#ifndef GLStuff_Renderer_3D_1L_Textured_h
#define GLStuff_Renderer_3D_1L_Textured_h

#include <stdio.h>
#include "glm_include.hpp"

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
	void render(Camera, DirectionalLight*, glm::mat4 &mtx_model);

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
