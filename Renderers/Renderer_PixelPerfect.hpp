#ifndef GLStuff_Renderer_PixelPerfect_h
#define GLStuff_Renderer_PixelPerfect_h

#include "GLHelpers.hpp"
#include "GLProg.hpp"
#include "Primitives.hpp"
#include "FilePaths_CPP.hpp"

struct Renderer_PixelPerfect {
	struct Attribs {
		enum { VertPos, TexCoord };
	};

	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};

	struct Sprite {
		unsigned int tex;
		v2 pos;
		v2 drawn_size;

		v2 pos_in_tex;
		v2 size_in_tex;  // nb size in text should be as a proportion of tex size
	};

	struct {
		unsigned int vertexPos, texCoord;
	} buffers;

	GLProg *prog = NULL;
	unsigned int vao = 0;
	int n_vertices = 0;
	std::vector<Sprite> sprites;   // NB sprites cannot currently be removed, since that would
	                               // invalid their user-side IDs.

	~Renderer_PixelPerfect() {
		VBO::destroy(buffers.vertexPos);
		VBO::destroy(buffers.texCoord);
		VAO::destroy(vao);
		if (prog) {
			delete prog;
		}
	}

  size_t add_sprite(Sprite s) {
    sprites.push_back(s);
    return sprites.size() - 1;
  }
  Sprite& get_sprite(size_t sprite_id) {
    return sprites[sprite_id];
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
			{ Uniforms::Sampler,     "uTex"         },
		};

		prog->compile();
		if (prog->state != GLProg::State::OK) {
			printf("oh dear");
			return false;
		}

		VAO::bind(0);
		return true;
	}
	
	void render(v2 win_size) {
		Prog::use(prog->programID);
		VAO::bind(vao);

		m4 ortho = glm::ortho(0.f, win_size.x, win_size.y, 0.f, 1.f, -1.f);
		Prog::set_uniform_mat4(Uniforms::OrthoMatrix, ortho);
		Prog::set_uniform_int(prog->uniformID(Uniforms::Sampler), 0);

		int i=0;
		unsigned int prev_tex = 0;
		for (const auto &s : sprites) {
			if (i++ == 0 || s.tex != prev_tex)
				Texture::bind(s.tex);
			prev_tex = s.tex;

			// Regenerate buffer
      std::vector<v2> rect = to_2d_primitive(Rectangle()) * v2(0.5, -0.5) + 0.5f;
      rect = rect * s.drawn_size + s.pos;
			VBO::bind(buffers.vertexPos, VBO::Array);
			VBO::upload(rect, VBO::Array, VBO::Static);

			VBO::bind(buffers.texCoord, VBO::Array);
      std::vector<v2> t = to_2d_primitive(Rectangle()) * v2(0.5, 0.5) * s.size_in_tex + s.pos_in_tex;  // TODO: replace Rectangle() with unitsq_texcoords
			VBO::upload(t, VBO::Array, VBO::Static);

			n_vertices = 6;
			glDrawArrays(GL_TRIANGLES, 0, n_vertices);
		}

		VAO::bind(0);
	}
};

#endif

// Original W Sprite class:
//class Sprite {
//public:
//	Sprite(View *, Texture *, const v2f &pos, const v2f &scale = v2f(1.0,1.0), float opacity = 1, float rotInDegrees = 0, int layer = DEFAULT_LAYER, BlendMode::T = BlendMode::Normal);
//	~Sprite();
//
//	void setPos(const v2f &);
//	void setScale(const v2f &);
//	void setRot(float);
//	void setAll(const v2f &pos, const v2f &sz, float);
//	void nudge(const v2f &);
//
//	void setOpacity(float);
//
//	void setLayer(int);
//	void setBlendMode(BlendMode::T);
//
//	v2f pos, scale;
//	float rot;
//
//	float opac;
//
//private:
//	void *dSprite;
//};
