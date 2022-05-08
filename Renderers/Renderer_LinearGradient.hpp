#ifndef ComponentTest__Renderer_LinearGradient_h
#define ComponentTest__Renderer_LinearGradient_h

#include "CoordinateTypes.hpp"

class Renderer_2D_ColourIndexed;

class Renderer_LinearGradient {
public:
	Renderer_LinearGradient(bool dither);
	~Renderer_LinearGradient();

	bool setUp();

	void render(float *mtx);
	void setPos(float x, float y, float w, float h);
	void setColours(v4 top, v4 btm);
	void setColours(v4 tl, v4 tr, v4 bl, v4 br);

	struct Uniforms {
		enum { OrthoMatrix, Sampler };
	};

private:
	Renderer_2D_ColourIndexed *renderer;

	v4 cTL, cTR, cBL, cBR;
	float x, y, w, h;
	bool dither;
	bool needs_upload;

	void reupload();

};

#endif
