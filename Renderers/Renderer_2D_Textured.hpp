#ifndef Renderer_2D_Textured_h
#define Renderer_2D_Textured_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "FilePaths_CPP.hpp"

class Renderer_2D_Textured {
public:
	struct Attribs {
		enum { VertPos, TexCoord };
	};
	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};
	struct {
		unsigned int vertexPos, texCoord;
	} buffers;

	int n_vertices = 0;
	GLProg *prog = NULL;
	unsigned int vao = 0;
	unsigned int tex = 0;

	~Renderer_2D_Textured() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.texCoord);
		VAO::destroy(vao);
		if (prog) {
			delete prog;
		}
	}

	bool setUp() {
		// VAO
		vao = VAO::create();
		VAO::bind(vao);

		buffers.vertexPos = VBO::create();
		buffers.texCoord  = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/2D_Textured.vsh");
		prog->fsh_path = bundledFilePath("Shaders/2D_Textured.fsh");
		prog->attribs = {
			{ Attribs::VertPos,  buffers.vertexPos, "inVPos",     Prog::Float, 2 },
			{ Attribs::TexCoord, buffers.texCoord,  "inTexCoord", Prog::Float, 2 }
		};
		prog->uniforms = {
			{ Uniforms::OrthoMatrix, "ortho_matrix" },
			{ Uniforms::Sampler, "uTex"    },
		};

		prog->compile();
		if (prog->state != GLProg::State::OK) {
			printf("oh dear");
			return false;
		}

		VAO::bind(0);
		return true;
	}
	
	void render(m4 ortho_mtx) {
		Prog::use(prog->programID);
		VAO::bind(vao);

		Texture::bind(0, tex);

		Prog::set_uniform_mat4(prog->uniformID(Uniforms::OrthoMatrix), ortho_mtx);
		Prog::set_uniform_int(prog->uniformID(Uniforms::Sampler), 0);

		glDrawArrays(GL_TRIANGLES, 0, n_vertices);

		VAO::bind(0);
	}
};

#endif
