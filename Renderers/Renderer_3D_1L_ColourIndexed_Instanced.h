#ifndef __GLStuff_Renderer_3D_1L_ColourIndexed_Instanced
#define __GLStuff_Renderer_3D_1L_ColourIndexed_Instanced

#include "CoordinateTypes.h"

class GLProg;
class Camera;
struct DirectionalLight;

#include "glm_include.h"


class Renderer_3D_1L_ColourIndexed_Instanced {
public:
	Renderer_3D_1L_ColourIndexed_Instanced();
	~Renderer_3D_1L_ColourIndexed_Instanced();

	bool setUp();
	void render(Camera cam, DirectionalLight *light, glm::mat3 &mtx_model);

	struct Attribs {
		enum { VertPos, Normal, Colour, Quaternion, Translation };
	};

	struct Uniforms {
		enum {
			VPMtx,           ModelMtx,      NormalMtx,
			LightVec,        LightProperties,
			ColToLightRatio,
			CamPos
		};
	};

	struct {
		unsigned int vertexPos, normal, colour, quaternion, translation;
	} buffers;

	int n_vertices_per_model;
	int n_models;

private:
	GLProg *prog;
	unsigned int vao;

	float colToLightRatio;

};

#endif

