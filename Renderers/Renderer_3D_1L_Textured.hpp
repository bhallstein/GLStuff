#ifndef GLStuff_Renderer_3D_1L_Textured_h
#define GLStuff_Renderer_3D_1L_Textured_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "ObjImport.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "FilePaths_CPP.hpp"

struct Renderer_3D_1L_Textured {
	struct Attribs {
		enum {
			VertPos, Normal, TexCoord, Elements,
		};
	};

	struct Uniforms {
		enum {
			MVPMtx, ModelMtx,  NormalMtx,
			Sampler,
			LightVec,  LightProperties,  ColToLightRatio,
			CamPos
		};
	};

	struct {
		unsigned int vertexPos, normal, texcoord, elements;
	} buffers;

	GLProg *prog = NULL;
	unsigned int vao = 0;
	int n_elements = 0;
	float colToLightRatio = 0.75;
	unsigned int tex = 0;

	~Renderer_3D_1L_Textured() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.normal);
		VBO::destroy(buffers.texcoord);
		VBO::destroy(buffers.elements);
		VAO::destroy(vao);
		if (prog) {
			delete prog;
		}
		// NB: The texture is assumed to be managed externally,
		//     so is not automatically deleted.
	}

	bool setUp() {
		vao = VAO::create();
		VAO::bind(vao);

		buffers.vertexPos   = VBO::create();
		buffers.normal      = VBO::create();
		buffers.texcoord    = VBO::create();
		buffers.elements    = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/3D_Textured.vsh");
		prog->fsh_path = bundledFilePath("Shaders/3D_Textured.fsh");
		prog->attribs = {
			{ Attribs::VertPos,     buffers.vertexPos,   "inVPos",     Prog::Float, 3, false },
			{ Attribs::Normal,      buffers.normal,      "inVNormal",  Prog::Float, 3, false },
			{ Attribs::TexCoord,    buffers.texcoord,    "inTexCoord", Prog::Float, 2, false },
		};
		prog->uniforms = {
			{ Uniforms::MVPMtx,          "mvpMtx" },
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

		n_elements = (int) obj.elements.size();
	}

	void render(Camera cam, DirectionalLight light, m4 m_model) {
		if (n_elements < 0) {
			printf("Renderer_3D_1L_Textured::render() called but n_elements is -1");
			return;
		}

		Prog::use(prog->programID);
		VAO::bind(vao);

		Texture::bind(tex);

		// Uniforms
		m4 m_view = cam.view_matrix();
		m4 m_proj = cam.proj_matrix;
		m4 m_mvp = m_proj * m_view * m_model;
		m3 m_normal = glm::inverse(glm::transpose(m3(m_model)));

		// mvp matrices
		Prog::set_uniform_mat4(prog->uniformID(Uniforms::MVPMtx), m_mvp);
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

		glDrawElements(GL_TRIANGLES, n_elements, GL_UNSIGNED_INT, 0);
		VAO::bind(0);
	}
};

#endif
