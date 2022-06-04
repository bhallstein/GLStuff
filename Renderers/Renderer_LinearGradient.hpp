#ifndef ComponentTest__Renderer_LinearGradient_h
#define ComponentTest__Renderer_LinearGradient_h

#include "Renderer_2D_ColourIndexed.hpp"

struct Renderer_LinearGradient {
	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};

	Renderer_2D_ColourIndexed *renderer = NULL;
	v4 cTL = { 1.0, 0.0, 0.0, 1.0 };  // N.B. After changing any of these props, set needs_upload to true
	v4 cTR = { 1.0, 0.0, 0.0, 1.0 };
	v4 cBL = { 1.0, 1.0, 0.0, 1.0 };
	v4 cBR = { 1.0, 1.0, 0.0, 1.0 };
  v2 pos = {0., 0.};
  v2 size = {0., 0.};
	bool dither = false;
	bool needs_upload = true;

	~Renderer_LinearGradient() {
		if (renderer) {
			delete renderer;
		}
	}

	bool setUp() {
		renderer = new Renderer_2D_ColourIndexed;
		renderer->dither = dither;
		return renderer->setUp();
	}

	void render(m4 mtx) {
		if (needs_upload) {
			reupload();
		}
		renderer->render(mtx);
	}

	void reupload() {
    Primitive2D rect = (Rectangle2D() + v2{0.5, 0.5}) * size + pos;
		std::vector<v4> colours = { cTR, cTL, cBL, cBL, cBR, cTR };

		VBO::bind(renderer->buffers.vertexPos, VBO::Array);
		VBO::upload(rect, VBO::Array, VBO::Static);

		VBO::bind(renderer->buffers.colour, VBO::Array);
		VBO::upload(colours, VBO::Array, VBO::Static);

		VAO::bind(0);
		needs_upload = false;
	}
};

#endif
