#ifndef GLStuff_Renderer_2D_ColourIndexed_h
#define GLStuff_Renderer_2D_ColourIndexed_h

#include "GLProg.hpp"
#include "GLHelpers.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"

struct Renderer_2D_ColourIndexed {
	struct Attribs {
		enum { VertPos, Colour };
	};

	struct Uniforms {
		enum { OrthoMatrix, Sampler_Dithering };
	};

	struct {
    unsigned int vertexPos;
    unsigned int colour;
	} buffers;

	int n_vertices = 0;
	GLProg *prog = NULL;
	unsigned int vao;
	bool dither = false;
	unsigned int tex_dither;

	~Renderer_2D_ColourIndexed() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.colour);
		VAO::destroy(vao);
		Texture::destroy(tex_dither);
		if (prog) {
			delete prog;
		}
	}

	bool setUp() {
		vao = VAO::create();
		VAO::bind(vao);

		buffers.vertexPos = VBO::create();
		buffers.colour = VBO::create();

		prog = new GLProg;
		prog->vsh_path = bundledFilePath("Shaders/2D_ColourIndexed.vsh");
		prog->fsh_path = bundledFilePath("Shaders/2D_ColourIndexed.fsh");
		prog->attribs = {
			{ Attribs::VertPos, buffers.vertexPos, "inVPos",   Prog::Float, 2, false },
			{ Attribs::Colour,  buffers.colour,    "inColour", Prog::Float, 4, false },
		};
		prog->uniforms = {
			{ Uniforms::OrthoMatrix, "ortho_matrix" },
		};
		if (dither) {
			prog->fsh_path = bundledFilePath("Shaders/2D_ColourIndexed_Dithered.fsh");
			prog->uniforms.push_back({ Uniforms::Sampler_Dithering, "uTex" });

			tex_dither = Texture::create();
			Texture::bind(tex_dither);
			Texture::set_repeat(true);
			Texture::set_filtering(Texture::FilterNearest);

			static const char pattern[] = {
				0,  32,  8, 40,  2, 34, 10, 42,   /* 8x8 Bayer ordered dithering  */
				48, 16, 56, 24, 50, 18, 58, 26,  /* pattern.  Each input pixel   */
				12, 44,  4, 36, 14, 46,  6, 38,  /* is scaled to the 0..63 range */
				60, 28, 52, 20, 62, 30, 54, 22,  /* before looking in this table */
				3,  35, 11, 43,  1, 33,  9, 41,   /* to determine the action.     */
				51, 19, 59, 27, 49, 17, 57, 25,
				15, 47,  7, 39, 13, 45,  5, 37,
				63, 31, 55, 23, 61, 29, 53, 21
			};
			glTexImage2D(GL_TEXTURE_2D,
									 0,        // mipmap
									 GL_RED,   // internal format
									 8,        // w
									 8,        // h
									 0,        // border
									 GL_RED,   // given format
									 GL_UNSIGNED_BYTE,  // given type
									 pattern);
			Texture::bind(0);
		}

		prog->compile();
		if (prog->state != GLProg::State::OK) {
			printf("Renderer_2D_ColourIndexed: Failed to compile program\n");
			VAO::bind(0);
			return false;
		}

    std::vector<v2> r = to_2d_primitive(Rectangle()) * v2{2., 2.};
		std::vector<v4> colours = {
			{ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 1.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 },
			{ 1.0, 0.0, 0.0, 1.0 }, { 0.0, 0.0, 1.0, 1.0 }, { 1.0, 1.0, 0.0, 1.0 }
		};

		VBO::bind(buffers.vertexPos, VBO::Array);
		VBO::upload(r, VBO::Array, VBO::Static);

    VBO::bind(buffers.colour, VBO::Array);
		VBO::upload(colours, VBO::Array, VBO::Static);

		n_vertices = 6;

		VAO::bind(0);
		return true;
	}

	void render(m4 ortho_mtx) {
		glDisable(GL_DEPTH_TEST);
		VAO::bind(vao);

		Prog::use(prog->programID);
		Prog::set_uniform_mat4(prog->uniformID(Uniforms::OrthoMatrix), ortho_mtx);

		if (dither) {
			Texture::bind(tex_dither);
			Prog::set_uniform_int(prog->uniformID(Uniforms::Sampler_Dithering), 0);
		}

		glDrawArrays(GL_TRIANGLES, 0, 6);
		VAO::bind(0);
	}
};

#endif
