#ifndef GLStuff_Renderer_3D_1L_Textured_Instanced_h
#define GLStuff_Renderer_3D_1L_Textured_Instanced_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "ObjImport.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "FilePaths_CPP.hpp"
#include "Primitives.hpp"

struct Renderer_3D_1L_Textured_Instanced {
	struct Attribs {
		enum { VertPos, Normal, TexCoord, Elements, Quaternion, Translation };
	};

	struct Uniforms {
		enum {
			VPMtx, ModelMtx,  NormalMtx,
			Sampler,
			LightVec,  LightProperties,  ColToLightRatio,
			CamPos
		};
	};

	struct {
		unsigned int vertexPos, normal, texcoord, elements, quaternion, translation;
	} buffers;

	GLProg *prog = NULL;
	unsigned int vao = 0;
	float colToLightRatio = 0.75;
	int n_elements_per_model = 0;
	int n_models = 0;
	unsigned int tex = 0;

	~Renderer_3D_1L_Textured_Instanced() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.normal);
		VBO::destroy(buffers.texcoord);
		VBO::destroy(buffers.elements);
		VBO::destroy(buffers.quaternion);
		VBO::destroy(buffers.translation);
		VAO::destroy(vao);
		if (prog) {
			delete prog;
		}
	}

	bool setUp() {
		vao = VAO::create();
		VAO::bind(vao);

		buffers.vertexPos   = VBO::create();
		buffers.normal      = VBO::create();
		buffers.texcoord    = VBO::create();
		buffers.elements    = VBO::create();
		buffers.translation = VBO::create();
		buffers.quaternion  = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/3D_Textured_Instanced.vsh");
		prog->fsh_path = bundledFilePath("Shaders/3D_Textured.fsh");
		prog->attribs = {
			{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",        Prog::Float, 3, false },
			{ Attribs::Normal,      buffers.normal,      "inVNormal",     Prog::Float, 3, false },
			{ Attribs::TexCoord,    buffers.texcoord,    "inTexCoord",    Prog::Float, 2, false },
			{ Attribs::Quaternion,  buffers.quaternion,  "inQuaternion",  Prog::Float, 4, true },
			{ Attribs::Translation, buffers.translation, "inTranslation", Prog::Float, 3, true },
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

		VBO::bind(buffers.elements, VBO::ElementArray);

		prog->compile();
		if (prog->state != GLProg::State::OK) {
			printf("oh dear");
			return false;
		}

		VAO::bind(0);
		return true;
	}

	void setObj(ObjFile &obj) {
		// VBOs
		int n_bytes = (int) (sizeof(v3) * obj.vertices.size());
		int n_bytes_uv = (int) (sizeof(v2) * obj.vertices.size());
		int n_bytes_elements = (int) (sizeof(int) * obj.elements.size());

		VBO::bind(buffers.vertexPos, VBO::Array);
		VBO::upload(n_bytes, &obj.vertices[0], VBO::Array, VBO::Static);

		VBO::bind(buffers.normal, VBO::Array);
		VBO::upload(n_bytes, &obj.normals[0], VBO::Array, VBO::Static);

		VBO::bind(buffers.texcoord, VBO::Array);
		VBO::upload(n_bytes_uv, &obj.texcoords[0], VBO::Array, VBO::Static);

		VBO::bind(buffers.elements, VBO::ElementArray);
		VBO::upload(n_bytes_elements, &obj.elements[0], VBO::ElementArray, VBO::Static);

		n_elements_per_model = (int) obj.elements.size();
	}
		
	// set sampler?
	void render(Camera cam, DirectionalLight light, m3 m_model) {
		Prog::use(prog->programID);
		VAO::bind(vao);

		Texture::bind(0, tex);

		// Uniforms

		// mvp matrices
		m4 m_view = cam.view_matrix();
		m4 m_proj = cam.proj_matrix;
		m4 m_vp   = m_proj * m_view;
		m3 m_normal = glm::inverse(glm::transpose(m_model));
		Prog::set_uniform_mat4(prog->uniformID(Uniforms::VPMtx), m_vp);
		Prog::set_uniform_mat3(prog->uniformID(Uniforms::ModelMtx), m_model);
		Prog::set_uniform_mat3(prog->uniformID(Uniforms::NormalMtx), m_normal);

		// tex
		Prog::set_uniform_int(prog->uniformID(Uniforms::Sampler), 0);

		// lights
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::LightProperties), light.lightProperties);
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::LightVec), light.lightVector);

		// colour
		Prog::set_uniform_float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

		// camera
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::CamPos), cam.pos);

		glDrawElementsInstanced(GL_TRIANGLES, n_elements_per_model, GL_UNSIGNED_INT, 0, n_models);
		VAO::bind(0);
	}
};

#endif
