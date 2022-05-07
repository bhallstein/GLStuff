#include "Renderer_3D_1L_ColourIndexed.h"
#include "GLProg.h"
#include "GLHelpers.h"
#include <OpenGL/gl.h>
#include "CoordinateTypes.h"
#include "glm_include.h"
#include "Camera.h"
#include "Lights.h"
#include "Primitives.h"
#include "FilePaths_CPP.h"


Renderer_3D_1L_ColourIndexed::Renderer_3D_1L_ColourIndexed() :
prog(NULL),
vao(0),
n_vertices(0),
colToLightRatio(0.75)
{

}

Renderer_3D_1L_ColourIndexed::~Renderer_3D_1L_ColourIndexed()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.normal);
	vbo_delete(buffers.colour);
	vao_delete(vao);
	if (prog) delete prog;
}


bool Renderer_3D_1L_ColourIndexed::setUp() {
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos = vbo_create();
	buffers.normal    = vbo_create();
	buffers.colour    = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_ColourIndexed.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_ColourIndexed.fsh");
	prog->attribs = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,  buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Colour,  buffers.colour,    "inColour",  ATTRTYPE_FLOAT, 3, false },
	};
	prog->uniforms = {
		{ Uniforms::MVPMtx,          "mvpMtx"           },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos,          "uCamPosition"     },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" }
	};

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*36, unitCube_vert.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*36, unitCube_norm.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

	// Colours
	v3 colours[36];
	std::vector<v3> side_colours = {
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 1.0, 0.0 },
		{ 0.0, 1.0, 1.0 },
		{ 1.0, 0.0, 1.0 }
	};
	for (int side=0; side < 6; ++side) {
		const v3 &col = side_colours[side];
		for (int i=0; i < 6; ++i)
			colours[side*6 + i] = col;
	}
	vbo_bind(buffers.colour, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*36, colours, VBOTYPE_ARRAY, VBOHINT_STATIC);

	n_vertices = 36;

	return true;
}

void Renderer_3D_1L_ColourIndexed::render(Camera cam, DirectionalLight *light, glm::mat4 &m_model) {
	prog_use(prog->programID);
	vao_bind(vao);

	// Uniforms

	// mvp matrices
	glm::mat4 m_view = cam.view_matrix();
	glm::mat4 m_proj = cam.proj_matrix;
	glm::mat4 m_mvp = m_proj * m_view * m_model;
	glm::mat3 m_normal = glm::inverse(glm::transpose(glm::mat3(m_model)));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::MVPMtx), mptr(m_mvp));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::ModelMtx), mptr(m_model));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::NormalMtx), mptr(m_normal));

	// lights
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightVec), light->lightVector);
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightProperties), light->lightProperties);

	// colour
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), glm::value_ptr(cam.pos));

	glDrawArrays(GL_TRIANGLES, 0, n_vertices);

	vao_bind(0);
}

void Renderer_3D_1L_ColourIndexed::setRatio(float x) { colToLightRatio = x; }


