#ifndef __GLStuff_Renderer_3D_1L_ReflectionMapped
#define __GLStuff_Renderer_3D_1L_ReflectionMapped

#include "CoordinateTypes.h"
#include "glm_include.h"

class GLProg;
class Camera;
struct DirectionalLight;


class Renderer_3D_1L_ReflectionMapped {
public:
	Renderer_3D_1L_ReflectionMapped();
	~Renderer_3D_1L_ReflectionMapped();
	
	bool setUp();
	void setTex(unsigned int);
	void render(Camera*, DirectionalLight*, glm::mat4 &m_model);
	
	struct Attribs {
		enum { VertPos, Normal };
	};
	struct Uniforms {
		enum {
			MVPMtx,      ModelMtx,        NormalMtx,
			LightVec,    LightProperties,
			Ratio,
			CamPos,
			CubeMapTexUnit
		};
	};
	
	struct {
		unsigned int vertexPos, normal;
	} buffers;
	
	int n_vertices;
	
private:
	GLProg *prog;
	unsigned int vao;
	unsigned int tex;
	
	float colToLightRatio;
	
};

#endif

