#ifndef GLStuff_Renderer_PixelCol_h
#define GLStuff_Renderer_PixelCol_h

#include "CoordinateTypes.hpp"
#include "Renderer_2D_ColourIndexed.hpp"

class GLProg;

class Renderer_PixelCol {
public:
	Renderer_PixelCol();
	~Renderer_PixelCol();

	bool setUp();
	void setRect(const v2 &pos, const v2 &size);
	void setColour(const col4 &);
	void render(v2 win_size);

private:
	Renderer_2D_ColourIndexed *rend;
	v2 _pos, _size;
	col4 _col;
};

#endif
