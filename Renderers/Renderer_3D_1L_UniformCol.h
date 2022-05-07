#ifndef __GLStuff_Renderer_3D_1L_UniformCol
#define __GLStuff_Renderer_3D_1L_UniformCol

#include "glm_include.h"
#include "CoordinateTypes.h"

class GLProg;
class Camera;
class DirectionalLight;


class Renderer_3D_1L_UniformCol {
public:
	Renderer_3D_1L_UniformCol();
	~Renderer_3D_1L_UniformCol();

	bool setUp();
	void setCol(v3);
	void setPrimitive(v3 *vertices,
										v3 *normals,
										int n_vertices);
	void render(Camera, DirectionalLight*, glm::mat4 &mtx_model);

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

	struct {
		unsigned int vertexPos, normal;
	} buffers;

	int n_vertices;

private:
	GLProg *prog;
	unsigned int vao;

	float colToLightRatio;
	v3 colour;

};


#endif

