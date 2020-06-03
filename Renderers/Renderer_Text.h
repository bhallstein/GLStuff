//
//  Renderer_Text.h
//
//  A sprite renderer. Sprites have locations and position set.
//
//


#ifndef __GLStuff_Renderer_Text
#define __GLStuff_Renderer_Text

#include "Renderer_PixelPerfect.h"


class Renderer_Text : public Renderer_PixelPerfect {
public:
	Renderer_Text();
	~Renderer_Text();
	
	bool setUp();
	void set(const char *,
			 v2 containerSize,
			 float fontSize,
			 col4 colour,
			 col4 bgColour);
	
//	bool setText(const char *);
//	bool setContainerSize(v2);
//	bool setfontSize(float);
//	bool setColor(col4);
//	bool setColor_BG(col4);
	
	void setPosition(v2);
	void render(v2 win_size);
	
private:
	void regen(const char *s, float, float, float, col4, col4);
	unsigned int tex;
	bool sprite_added;
	unsigned int sprite_id;
};

#endif

