#ifndef Renderer_3D_1L_UniformCol_Instanced_h
#define Renderer_3D_1L_UniformCol_Instanced_h

#include "glm_include.hpp"
#include "CoordinateTypes.hpp"

class GLProg;
class Camera;
class DirectionalLight;

class Renderer_3D_1L_UniformCol_Instanced {
public:
	Renderer_3D_1L_UniformCol_Instanced();
	~Renderer_3D_1L_UniformCol_Instanced();

	bool setUp();
	void setCol(v3);
	void setPrimitive(v3 *vertices,
										v3 *normals,
										int n_vertices);
	void render(Camera, DirectionalLight*, glm::mat3 &mtx_model);

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
