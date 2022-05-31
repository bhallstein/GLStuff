#ifndef GLStuff_Renderer_3D_1L_UniformCol_h
#define GLStuff_Renderer_3D_1L_UniformCol_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"

struct Renderer_3D_1L_UniformCol {
	struct Attribs {
		enum {  VertPos, Normal  };
	};

	struct Uniforms {
		enum {
			MVPMtx,    ModelMtx,         NormalMtx,
			LightVec,  LightProperties,
			Colour,    ColToLightRatio,
			CamPos
		};
	};

	struct {
		unsigned int vertexPos, normal;
	} buffers;

	GLProg *prog = NULL;
	unsigned int vao = 0;
	int n_vertices = 0;
	float colToLightRatio = 0.75;
	v3 colour = { 1.0, 0.75, 0.25 };

	~Renderer_3D_1L_UniformCol() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.normal);
		VAO::destroy(vao);
		if (prog) {
			delete prog;
		}
	}

	bool setUp() {
		vao = VAO::create();
		VAO::bind(vao);

		buffers.vertexPos = VBO::create();
		buffers.normal    = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/3D_UniformColour.vsh");
		prog->fsh_path = bundledFilePath("Shaders/3D_UniformColour.fsh");
		prog->attribs = {
			{ Attribs::VertPos, buffers.vertexPos, "inVPos",    Prog::Float, 3, false },
			{ Attribs::Normal,  buffers.normal,    "inVNormal", Prog::Float, 3, false }
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

    Primitive3D cube = Cube();
		setPrimitive(cube.vertices, cube.normals);

		VAO::bind(0);
		return true;
	}

	void setPrimitive(const std::vector<v3> &vertices, const std::vector<v3> &normals) {
		n_vertices = (int) vertices.size();

		VBO::bind(buffers.vertexPos, VBO::Array);
		VBO::upload(vertices, VBO::Array, VBO::Static);

		VBO::bind(buffers.normal, VBO::Array);
		VBO::upload(normals, VBO::Array, VBO::Static);

		VBO::bind(0, VBO::Array);
	}

	void render(Camera cam, DirectionalLight light, m4 m_model) {
		Prog::use(prog->programID);
		VAO::bind(vao);

		// Uniforms

		// mvp matrices
		m4 m_view = cam.view_matrix();
		m4 m_proj = cam.proj_matrix;
		m4 m_mvp = m_proj * m_view * m_model;
		m3 m_normal = glm::inverse(glm::transpose(m3(m_model)));
		Prog::set_uniform_mat4(prog->uniformID(Uniforms::MVPMtx), m_mvp);
		Prog::set_uniform_mat4(prog->uniformID(Uniforms::ModelMtx), m_model);
		Prog::set_uniform_mat3(prog->uniformID(Uniforms::NormalMtx), m_normal);

		// lights
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::LightVec), light.lightVector);
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::LightProperties), light.lightProperties);

		// colour
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::Colour), colour);
		Prog::set_uniform_float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

		// camera
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::CamPos), cam.pos);

		glDrawArrays(GL_TRIANGLES, 0, n_vertices);

		VAO::bind(0);
	}
};

#endif
