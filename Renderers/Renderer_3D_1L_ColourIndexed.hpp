#ifndef GLStuff_Renderer_3D_1L_ColourIndexed_h
#define GLStuff_Renderer_3D_1L_ColourIndexed_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "Camera.hpp"
#include "Lights.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"


struct Renderer_3D_1L_ColourIndexed {
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

	GLProg *prog = NULL;
	unsigned int vao = 0;
	int n_vertices = 0;
	float colToLightRatio = 0.75;

	~Renderer_3D_1L_ColourIndexed() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.normal);
		VBO::destroy(buffers.colour);
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
		buffers.colour    = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/3D_ColourIndexed.vsh");
		prog->fsh_path = bundledFilePath("Shaders/3D_ColourIndexed.fsh");
		prog->attribs = {
			{ Attribs::VertPos, buffers.vertexPos, "inVPos",    Prog::Float, 3, false },
			{ Attribs::Normal,  buffers.normal,    "inVNormal", Prog::Float, 3, false },
			{ Attribs::Colour,  buffers.colour,    "inColour",  Prog::Float, 3, false },
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

    Primitive3D cube = Cube();

		VBO::bind(buffers.vertexPos, VBO::Array);
		VBO::upload(cube.vertices, VBO::Array, VBO::Static);

		VBO::bind(buffers.normal, VBO::Array);
		VBO::upload(cube.normals, VBO::Array, VBO::Static);

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
      for (int i=0; i < 6; ++i) {
				colours[side*6 + i] = col;
      }
		}
		VBO::bind(buffers.colour, VBO::Array);
		VBO::upload(sizeof(v3)*36, colours, VBO::Array, VBO::Static);

		n_vertices = 36;

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
		Prog::set_uniform_float(prog->uniformID(Uniforms::ColToLightRatio), colToLightRatio);

		// camera
		Prog::set_uniform_vec3(prog->uniformID(Uniforms::CamPos), cam.pos);

		glDrawArrays(GL_TRIANGLES, 0, n_vertices);

		VAO::bind(0);
	}

	void setRatio(float);
};

#endif
