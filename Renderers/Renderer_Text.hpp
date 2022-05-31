#ifndef GLStuff_Renderer_Text_h
#define GLStuff_Renderer_Text_h

#include "Renderer_PixelPerfect.hpp"
#include "TextTex.hpp"

struct Renderer_Text : public Renderer_PixelPerfect {
	unsigned int tex = 0;
	bool sprite_added = false;
	size_t sprite_id = -1;

	bool setUp() {
		bool parent_succceed = Renderer_PixelPerfect::setUp();
		if (!parent_succceed) return false;

		regen("muffins", 200, 60, 40, {1, 0, 0, 1}, {1, 1, 1, 1});
		return true;
	}
	
	void set(const char *s,
					 v2 containerSize,
					 float fontSize,
					 v4 colour,
					 v4 bgColour) {
		regen(s, containerSize.x, containerSize.y, fontSize, colour, bgColour);
	}

	//	bool setText(const char *);
	//	bool setContainerSize(v2);
	//	bool setfontSize(float);
	//	bool setColor(col4);
	//	bool setColor_BG(col4);

	void setPosition(v2 pos) {
		if (sprite_added) {
			Sprite &spr = Renderer_PixelPerfect::get_sprite(sprite_id);
			spr.pos = pos;
		}
	}
	
	void render(v2 win_size) {
		Renderer_PixelPerfect::render(win_size);
	}

	void regen(const char *s, float containerW, float containerH, float font_size, v4 col, v4 bgcol) {
		TexImage im = get_textTex(s, containerW, containerH, font_size, col, bgcol);
		if (im.data == NULL) return;

		if (!sprite_added) {
			tex = Texture::create();
		}
		Texture::bind(tex);
		Texture::upload(im.w, im.h, im.data, Texture::FilterLinear);
		free(im.data);

		if (!sprite_added) {
			sprite_id = Renderer_PixelPerfect::add_sprite({
				tex,
				{0, 0},
				{containerW, containerH},
				{0, 0},
				{1, 1}
			});
		}
		else {
			auto &spr = Renderer_PixelPerfect::get_sprite(sprite_id);
			spr.drawn_size = { containerW, containerH };
		}
		sprite_added = true;
	}
};

#endif
