#include <OpenGL/gl3.h>
#include "Renderer_Text.hpp"
#include "GLProg.hpp"
#include "CoordinateTypes.hpp"
#include "Primitives.hpp"
#include "GLHelpers.hpp"
#include "FilePaths_CPP.hpp"
#include "TextTex.hpp"
#include "glm_include.hpp"

Renderer_Text::Renderer_Text() : sprite_added(false)
{

}


Renderer_Text::~Renderer_Text()
{

}


bool Renderer_Text::setUp() {
	bool parent_succceed = Renderer_PixelPerfect::setUp();
	if (!parent_succceed) return false;

	regen("muffins", 200, 60, 40, {1, 0, 0, 1}, {1, 1, 1, 1});
	return true;
}

void Renderer_Text::set(const char *s,
												v2 containerSize,
												float fontSize,
												col4 colour,
												col4 bgColour) {
	regen(s, containerSize.x, containerSize.y, fontSize, colour, bgColour);
}

void Renderer_Text::setPosition(v2 pos) {
	if (sprite_added) {
		Sprite &spr = Renderer_PixelPerfect::getSprite(sprite_id);
		spr.pos = pos;
	}
}

void Renderer_Text::render(v2 win_size) {
	Renderer_PixelPerfect::render(win_size);
}

void Renderer_Text::regen(const char *s, float containerW, float containerH, float font_size, col4 c, col4 bg) {
	TexImage im = get_textTex(s, containerW, containerH, font_size, c, bg);
	if (im.data == NULL) return;

	if (!sprite_added) {
		tex = tx_create();
	}
	tx_bind(tex);
	tx_upload(im.w, im.h, im.data, TX_FILTER_LINEAR);
	free(im.data);

	if (!sprite_added) {
		sprite_id = Renderer_PixelPerfect::addSprite({
			tex,
			{0, 0},
			{containerW, containerH},
			{0, 0},
			{1, 1}
		});
	}
	else {
		auto &spr = Renderer_PixelPerfect::getSprite(sprite_id);
		spr.drawn_size = { containerW, containerH };
	}
	sprite_added = true;
}
