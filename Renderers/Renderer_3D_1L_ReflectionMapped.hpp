#ifndef GLStuff_Renderer_3D_1L_ReflectionMapped_h
#define GLStuff_Renderer_3D_1L_ReflectionMapped_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "FilePaths_CPP.hpp"
#include "Primitives.hpp"

struct Renderer_3D_1L_ReflectionMapped {
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

	GLProg *prog = NULL;
	unsigned int vao = 0;
	unsigned int tex = 0;
	int n_vertices = 0;
	float colToLightRatio = 0.75;

	~Renderer_3D_1L_ReflectionMapped() {
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

		buffers.vertexPos  = VBO::create();
		buffers.normal     = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/3D_RM.vsh");
		prog->fsh_path = bundledFilePath("Shaders/3D_RM.fsh");
		prog->attribs = {
			{ Attribs::VertPos,  buffers.vertexPos, "inVPos",    Prog::Float, 3 },
			{ Attribs::Normal,   buffers.normal,    "inVNormal", Prog::Float, 3 }
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

    Primitive3D sq_pyr = SquarePyramid();
    sq_pyr.vertices = sq_pyr.vertices * v3{2.1, 1.33, 1.};
    sq_pyr.normals = mk_flat_normals(sq_pyr.vertices);

		VBO::bind(buffers.vertexPos, VBO::Array);
		VBO::upload(sq_pyr.vertices, VBO::Array, VBO::Static);

		VBO::bind(buffers.normal, VBO::Array);
		VBO::upload(sq_pyr.normals, VBO::Array, VBO::Static);

		n_vertices = 18;

		return true;
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
		Prog::set_uniform_float(prog->uniformID(Uniforms::Ratio), colToLightRatio);

		// camera
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::CamPos), cam.pos);

		// texture
		Prog::set_uniform_int(prog->uniformID(Uniforms::CubeMapTexUnit), 0);

		glDrawArrays(GL_TRIANGLES, 0, n_vertices);

		VAO::bind(0);
	}
};

#endif
