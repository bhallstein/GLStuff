#ifndef GLStuff_Renderer_3D_1L_ColourIndexed_h
#define GLStuff_Renderer_3D_1L_ColourIndexed_h

#include "CoordinateTypes.hpp"

class GLProg;
class Camera;
struct DirectionalLight;

#include "glm_include.hpp"


class Renderer_3D_1L_ColourIndexed {
public:
	Renderer_3D_1L_ColourIndexed();
	~Renderer_3D_1L_ColourIndexed();

	bool setUp();
	void render(Camera cam, DirectionalLight *light, glm::mat4 &m_model);

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
