#include "Renderer_PixelCol.h"
#include "Renderer_2D_ColourIndexed.h"
#include "GLHelpers.h"
#include "Primitives.h"
#include "glm_include.h"


Renderer_PixelCol::Renderer_PixelCol()
{
	rend = new Renderer_2D_ColourIndexed(false);
}
Renderer_PixelCol::~Renderer_PixelCol()
{
	delete rend;
}
	
bool Renderer_PixelCol::setUp() {
	return rend->setUp();
}

void Renderer_PixelCol::setRect(const v2 &pos, const v2 &size ) {
	Rectangle2D r = (unitSquare_2D + 0.5) * size + pos;
	vbo_bind(rend->buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v2) * 6, r.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);
	_pos = pos;
	_size = size;
}
void Renderer_PixelCol::setColour(const col4 &c) {
	col4 arr[] = {
		c, c, c, c, c, c
	};
	vbo_bind(rend->buffers.colour, VBOTYPE_ARRAY);
	vbo_upload(sizeof(col4) * 6, arr, VBOTYPE_ARRAY, VBOHINT_STATIC);
	_col = c;
}

void Renderer_PixelCol::render(v2 win_size) {
	glm::mat4 ortho = glm::ortho(0.f, win_size.x, win_size.y, 0.f, 1.f, -1.f);
	rend->render(mptr(ortho));
}

