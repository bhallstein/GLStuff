#include "Renderer_3D_1L_ReflectionMapped.h"
#include "GLProg.h"
#include "GLHelpers.h"
#include <OpenGL/gl.h>
#include "CoordinateTypes.h"
#include "Camera.h"
#include "Lights.h"
#include "FilePaths_CPP.h"
#include "Primitives.h"


Renderer_3D_1L_ReflectionMapped::Renderer_3D_1L_ReflectionMapped() :
prog(NULL),
vao(0),
tex(0),
n_vertices(0),
colToLightRatio(0.75)

{

}

Renderer_3D_1L_ReflectionMapped::~Renderer_3D_1L_ReflectionMapped()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.normal);
	vao_delete(vao);
	if (prog) delete prog;
}


bool Renderer_3D_1L_ReflectionMapped::setUp() {
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos  = vbo_create();
	buffers.normal     = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_RM.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_RM.fsh");
	prog->attribs = {
		{ Attribs::VertPos,  buffers.vertexPos, "inVPos",    ATTRTYPE_FLOAT, 3 },
		{ Attribs::Normal,   buffers.normal,    "inVNormal", ATTRTYPE_FLOAT, 3 }
	};
	prog->uniforms = {
		{ Uniforms::MVPMtx,          "mvpMtx"            },
		{ Uniforms::ModelMtx,        "modelMtx"          },
		{ Uniforms::NormalMtx,       "normalMtx"         },
		{ Uniforms::LightVec,        "uLightVector"      },
		{ Uniforms::LightProperties, "uLightProperties"  },
		{ Uniforms::CamPos,          "uCamPosition"      },
		{ Uniforms::Ratio,           "uReflToLightRatio" },
		{ Uniforms::CubeMapTexUnit,  "uCubeMapTexUnit"   },
	};

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	SquarePyramid pyr = unitSqPyramid * v2{ 2.1, 1.33 };

	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*18, pyr.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v3)*18, pyr.normals, VBOTYPE_ARRAY, VBOHINT_STATIC);

	n_vertices = 18;

	return true;
}

void Renderer_3D_1L_ReflectionMapped::setTex(unsigned int _tex) {
	tex = _tex;
}

void Renderer_3D_1L_ReflectionMapped::render(Camera cam, DirectionalLight *light, glm::mat4 &m_model) {
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
	prog_setUniformValue_Float(prog->uniformID(Uniforms::Ratio), colToLightRatio);

	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), glm::value_ptr(cam.pos));

	// texture
	prog_setUniformValue_Int(prog->uniformID(Uniforms::CubeMapTexUnit), 0);

	glDrawArrays(GL_TRIANGLES, 0, n_vertices);

	vao_bind(0);
}


