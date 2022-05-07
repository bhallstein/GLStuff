//
//  Renderer_LinearGradient.h
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __ComponentTest__Renderer_LinearGradient
#define __ComponentTest__Renderer_LinearGradient

class Renderer_2D_ColourIndexed;
#include "CoordinateTypes.h"


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
