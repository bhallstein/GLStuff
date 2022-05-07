//
//  Renderer_PixelPerfect.h
//
//  A sprite renderer. Sprites have locations and position set.
//
//

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


#ifndef __GLStuff_Renderer_PixelPerfect
#define __GLStuff_Renderer_PixelPerfect

#include "CoordinateTypes.h"
#include <vector>

class GLProg;


class Renderer_PixelPerfect {
public:
	Renderer_PixelPerfect();
	~Renderer_PixelPerfect();

	bool setUp();
	void render(v2 win_size);

	// Sprite stuff
	struct Sprite {
		unsigned int tex;
		v2 pos;
		v2 drawn_size;

		v2 pos_in_tex;
		v2 size_in_tex;
		// nb size in text should be as a proportion of tex size
	};
	unsigned int addSprite(Sprite);
	Sprite& getSprite(unsigned int spr_id) {
		return sprites[spr_id];
	}

	// Renderer stuff
	struct Attribs {
		enum { VertPos, TexCoord };
	};
	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};
	struct {
		unsigned int vertexPos, texCoord;
	} buffers;

	int n_vertices;

private:
	GLProg *prog;
	unsigned int vao;

	// Manage a list of sprites
	std::vector<Sprite> sprites;
	// NB sprites cannot atm be removed, since that would
	// invalid their user-side IDs.

};

#endif

