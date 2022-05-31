#ifndef GLStuff_Renderer_PixelCol_h
#define GLStuff_Renderer_PixelCol_h

#include "Renderer_2D_ColourIndexed.hpp"

struct Renderer_PixelCol {
  Renderer_2D_ColourIndexed *renderer;
  v2 _pos, _size;
  v4 _col;

	Renderer_PixelCol() {
		renderer = new Renderer_2D_ColourIndexed;
		renderer->dither = false;
	}

	~Renderer_PixelCol() {
		delete renderer;
	}

	bool setUp() {
		return renderer->setUp();
	}

	void setRect(v2 pos, v2 size) {
    std::vector<v2> r = (to_2d_primitive(Rectangle()) * v2(0.5, 0.5) + 0.5f) * size + pos;
		VBO::bind(renderer->buffers.vertexPos, VBO::Array);
		VBO::upload(r, VBO::Array, VBO::Static);
		_pos = pos;
		_size = size;
	}

	void setColour(v4 c) {
		std::vector<v4> arr = { c, c, c, c, c, c };
		VBO::bind(renderer->buffers.colour, VBO::Array);
		VBO::upload(arr, VBO::Array, VBO::Static);
		_col = c;
	}

	void render(v2 win_size) {
		m4 ortho = glm::ortho(0.f, win_size.x, win_size.y, 0.f, 1.f, -1.f);
		renderer->render(ortho);
	}
};

#endif
