#include "Renderer_3D_1L_UniformCol.hpp"
#include <OpenGL/gl3.h>
#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"

Renderer_3D_1L_UniformCol::Renderer_3D_1L_UniformCol() :
prog(NULL),
vao(0),
colour{ 1.0, 0.75, 0.25 },
colToLightRatio(0.75)
{

}

Renderer_3D_1L_UniformCol::~Renderer_3D_1L_UniformCol()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.normal);
	vao_delete(vao);
	if (prog) delete prog;
}

bool Renderer_3D_1L_UniformCol::setUp() {
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos = vbo_create();
	buffers.normal    = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_UniformColour.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_UniformColour.fsh");
	prog->attribs = {
		{ Attribs::VertPos, buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,  buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3, false }
	};
	prog->uniforms = {
		{ Uniforms::MVPMtx,          "mvpMtx"           },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::CamPos,          "uCamPosition"     },
		{ Uniforms::Colour,          "uColour"          },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" },
	};

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	setPrimitive(unitCube_vert.vertices, unitCube_norm.vertices, 36);

	vao_bind(0);
	return true;
}

void Renderer_3D_1L_UniformCol::setCol(v3 c) {
	colour = c;
}
void Renderer_3D_1L_UniformCol::setPrimitive(v3 *vertices, v3 *normals, int _n_vertices) {
	n_vertices = _n_vertices;

	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*n_vertices, vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*n_vertices, normals, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(0, VBOTYPE_ARRAY);
}

void Renderer_3D_1L_UniformCol::render(Camera cam, DirectionalLight *light, glm::mat4 &m_model) {
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
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::Colour), (float*) &colour);
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), glm::value_ptr(cam.pos));

	glDrawArrays(GL_TRIANGLES, 0, n_vertices);

	vao_bind(0);
}
