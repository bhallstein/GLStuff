#include "Renderer_3D_1L_Textured_Instanced.hpp"
#include "GLProg.hpp"
#include "ObjImport.hpp"
#include <OpenGL/gl3.h>
#include "Camera.hpp"
#include "Lights.hpp"
#include "FilePaths_CPP.hpp"
#include "Primitives.hpp"

Renderer_3D_1L_Textured_Instanced::Renderer_3D_1L_Textured_Instanced() :
prog(NULL),
vao(0),
tex(0),
n_elements_per_model(-1),
colToLightRatio(0.75)
{
	// hi
}

Renderer_3D_1L_Textured_Instanced::~Renderer_3D_1L_Textured_Instanced()
{
	vbo_delete(buffers.vertexPos);
	vbo_delete(buffers.normal);
	vbo_delete(buffers.texcoord);
	vbo_delete(buffers.elements);
	vbo_delete(buffers.quaternion);
	vbo_delete(buffers.translation);
	vao_delete(vao);
	if (prog) delete prog;
}

bool Renderer_3D_1L_Textured_Instanced::setUp() {
	vao = vao_create();
	vao_bind(vao);

	buffers.vertexPos   = vbo_create();
	buffers.normal      = vbo_create();
	buffers.texcoord    = vbo_create();
	buffers.elements    = vbo_create();
	buffers.translation = vbo_create();
	buffers.quaternion  = vbo_create();

	prog = new GLProg;
	prog->vsh_path = bundledFilePath("Shaders/3D_Textured_Instanced.vsh");
	prog->fsh_path = bundledFilePath("Shaders/3D_Textured.fsh");
	prog->attribs = {
		{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     ATTRTYPE_FLOAT, 3, false },
		{ Attribs::Normal,      buffers.normal,      "inVNormal",  ATTRTYPE_FLOAT, 3, false },
		{ Attribs::TexCoord,    buffers.texcoord,    "inTexCoord", ATTRTYPE_FLOAT, 2, false },
		{ Attribs::Quaternion,  buffers.quaternion,  "inQuaternion", ATTRTYPE_FLOAT, 4, true },
		{ Attribs::Translation, buffers.translation, "inTranslation", ATTRTYPE_FLOAT, 3, true },
	};
	prog->uniforms = {
		{ Uniforms::VPMtx,           "vpMtx"            },
		{ Uniforms::ModelMtx,        "modelMtx"         },
		{ Uniforms::NormalMtx,       "normalMtx"        },
		{ Uniforms::Sampler,         "uTex"             },
		{ Uniforms::LightVec,        "uLightVector"     },
		{ Uniforms::LightProperties, "uLightProperties" },
		{ Uniforms::ColToLightRatio, "uColToLightRatio" },
		{ Uniforms::CamPos,          "uCamPosition"     },
	};

	vbo_bind(buffers.elements, VBOTYPE_ELEMENT_ARRAY);

	prog->compile();
	if (prog->state != GLProg::State::OK) {
		printf("oh dear");
		return false;
	}

	vao_bind(0);
	return true;
}
void Renderer_3D_1L_Textured_Instanced::setObj(ObjFile *obj) {
	// VBOs
	int n_bytes = (int) (sizeof(v3) * obj->vertices.size());
	int n_bytes_uv = (int) (sizeof(v2) * obj->vertices.size());
	int n_bytes_elements = (int) (sizeof(int) * obj->elements.size());

	vbo_bind(buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(n_bytes, &obj->vertices[0], VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.normal, VBOTYPE_ARRAY);
	vbo_upload(n_bytes, &obj->normals[0], VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.texcoord, VBOTYPE_ARRAY);
	vbo_upload(n_bytes_uv, &obj->texcoords[0], VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(buffers.elements, VBOTYPE_ELEMENT_ARRAY);
	vbo_upload(n_bytes_elements, &obj->elements[0], VBOTYPE_ELEMENT_ARRAY, VBOHINT_STATIC);

	n_elements_per_model = (int) obj->elements.size();
}
void Renderer_3D_1L_Textured_Instanced::setTex(unsigned int _tex) {
	tex = _tex;
}

void Renderer_3D_1L_Textured_Instanced::render(Camera cam, DirectionalLight *light, glm::mat3 &m_model) {
	prog_use(prog->programID);
	vao_bind(vao);

	tx_bind(tex);

	// Uniforms

	// mvp matrices
	glm::mat4 m_view = cam.view_matrix();
	glm::mat4 m_proj = cam.proj_matrix;
	glm::mat4 m_vp   = m_proj * m_view;
	glm::mat3 m_normal = glm::inverse(glm::transpose(m_model));
	prog_setUniformValue_Mat4(prog->uniformID(Uniforms::VPMtx), mptr(m_vp));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::ModelMtx), mptr(m_model));
	prog_setUniformValue_Mat3(prog->uniformID(Uniforms::NormalMtx), mptr(m_normal));

	// tex
	prog_setUniformValue_Int(prog->uniformID(Uniforms::Sampler), 0);

	// lights
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightProperties), light->lightProperties);
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::LightVec), light->lightVector);

	// colour
	prog_setUniformValue_Float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

	// camera
	prog_setUniformValue_Vec3(prog->uniformID(Uniforms::CamPos), glm::value_ptr(cam.pos));


	glDrawElementsInstanced(GL_TRIANGLES, n_elements_per_model, GL_UNSIGNED_INT, 0, n_models);

	vao_bind(0);
}
