//
//  Renderer.cpp
//  ComponentTest
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#include "Renderer_LinearGradient.h"
#include "Renderer_2D_ColourIndexed.h"

#include "GLProg.h"

#include "glm_include.h"
//#include <OpenGL/gl3.h>
#include "Primitives.h"

#include "FilePaths_CPP.h"


Renderer_LinearGradient::Renderer_LinearGradient(bool _dither) :
renderer(NULL),
x(0.0),
y(0.0),
w(100.0),
h(100.0),
cTL{ 1.0, 0.0, 0.0, 1.0 },
cTR{ 1.0, 0.0, 0.0, 1.0 },
cBL{ 1.0, 1.0, 0.0, 1.0 },
cBR{ 1.0, 1.0, 0.0, 1.0 },
dither(_dither),
needs_upload(true)
{

}

Renderer_LinearGradient::~Renderer_LinearGradient()
{
	if (renderer) delete renderer;
}

bool Renderer_LinearGradient::setUp() {
	renderer = new Renderer_2D_ColourIndexed(dither);
	return renderer->setUp();
}
void Renderer_LinearGradient::reupload() {
	Rectangle2D r = unitSquare;
	r = (r + v2{0.5, 0.5}) * v2{w, h} + v2{x, y};

	v4 colours[] = {
		cBL, cTL, cTR,
		cBL, cTR, cBR,
	};

	vbo_bind(renderer->buffers.vertexPos, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v2) * 6, r.vertices, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vbo_bind(renderer->buffers.colour, VBOTYPE_ARRAY);
	vbo_upload(sizeof(v4) * 6, colours, VBOTYPE_ARRAY, VBOHINT_STATIC);

	vao_bind(0);
	needs_upload = false;
}


void Renderer_LinearGradient::setPos(float _x, float _y, float _w, float _h) {
	x = _x;
	y = _y;
	w = _w;
	h = _h;
	needs_upload = true;
}

void Renderer_LinearGradient::setColours(v4 top, v4 btm) {
	cTL = cTR = top;
	cBL = cBR = btm;
	needs_upload = true;
}

void Renderer_LinearGradient::setColours(v4 tl, v4 tr, v4 bl, v4 br) {
	cTL = tl;
	cTR = tr;
	cBL = bl;
	cBR = br;
	needs_upload = true;
}

void Renderer_LinearGradient::render(float *mtx) {
	if (needs_upload) reupload();

	renderer->render(mtx);
}


