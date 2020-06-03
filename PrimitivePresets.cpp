//
//  PrimitivePresets.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

//#include <stdio.h>
#include "PrimitivePresets.h"
#include "CoordinateTypes.h"
#include "math.h"

//static v3f _zero3f = { 0.0, 0.0, 0.0 };

static v3f _unit_equilateral_triangle[] = {
	{  0.0,     1.0,    0.0  },
	{  0.8142, -0.5806, 0.0  },
	{ -0.8142, -0.5806, 0.0  }
};


#pragma mark - Primitive struct

Primitive::Primitive(int _nVertices) : nVertices(_nVertices)
{
	vertices = new v3f[nVertices];
	normals = new v3f[nVertices];
}
Primitive::~Primitive()
{
	delete [] vertices;
	delete [] normals;
}

Primitive2D::Primitive2D(int _nVertices) : nVertices(_nVertices)
{
	vertices = new v2f[nVertices];
}
Primitive2D::~Primitive2D()
{
	delete [] vertices;
}


#pragma mark - Loading fns

Primitive pr_loadTriangle_Equil(float radius) {
	Primitive p(3);
	for (int i=0; i < 3; ++i) {
		p.vertices[i] = _unit_equilateral_triangle[i];
		p.normals[i] = { 0.0, 0.0, -1.0 };
	}
	return p;
};
Primitive2D pr_loadTriangle_Equil_2D(float radius) {
	Primitive2D p(3);
	for (int i=0; i < 3; ++i) {
		const v3f &c = _unit_equilateral_triangle[i];
		p.vertices[i] = { c.x, c.y };
	}
	return p;
}

Primitive pr_loadTriangle_Isosc(float w, float h) {
	Primitive p(3);
	
	p.vertices[0] = { 0.0, h, 0.0 };
	p.vertices[1] = {  w * 0.5f, 0.0, 0.0 };
	p.vertices[2] = { -w * 0.5f, 0.0, 0.0 };
	p.normals[0] = p.normals[1] = p.normals[2] = { 0.0, 0.0, -1.0 };
	
	return p;
}
Primitive2D pr_loadTriangle_Isosc_2D(float w, float h) {
	Primitive2D p(3);

	p.vertices[0] = {  0.0, h };
	p.vertices[1] = {  w * 0.5f, 0.0 };
	p.vertices[2] = { -w * 0.5f, 0.0 };
	
	return p;
}

Primitive pr_loadRectangle(float w, float h) {
	Primitive p(6);
	
	float x = 0.5 * w, y = 0.5 * h;
	p.vertices[0] = { -x, -y, 0.0 };
	p.vertices[1] = { -x,  y, 0.0 };
	p.vertices[2] = {  x,  y, 0.0 };
	p.vertices[3] = { -x, -y, 0.0 };
	p.vertices[4] = {  x,  y, 0.0 };
	p.vertices[5] = {  x, -y, 0.0 };
	
	for (int i=0; i < 6; ++i) p.normals[i] = { 0.0, 0.0, -1.0 };
	return p;
}
Primitive2D pr_loadRectangle_2D(float w, float h) {
	Primitive2D p(6);

	float x = 0.5 * w, y = 0.5 * h;
	p.vertices[0] = { -x, -y };
	p.vertices[1] = { -x,  y };
	p.vertices[2] = {  x,  y };
	p.vertices[3] = { -x, -y };
	p.vertices[4] = {  x,  y };
	p.vertices[5] = {  x, -y };
	
	return p;
}

Primitive pr_loadCuboid(float w, float h, float d) {
	Primitive p(36);
	
	int offs;
	float x = w*0.5, y = h*0.5, z = d*0.5;
	
	offs = 0;								// Front face
	p.vertices[0 + offs] = { -x, -y,  -z};   p.normals[0 + offs] = { 0.0, 0.0, -1.0 };
	p.vertices[1 + offs] = { -x,  y,  -z };  p.normals[1 + offs] = { 0.0, 0.0, -1.0 };
	p.vertices[2 + offs] = {  x,  y,  -z };  p.normals[2 + offs] = { 0.0, 0.0, -1.0 };
	p.vertices[3 + offs] = { -x, -y,  -z };  p.normals[3 + offs] = { 0.0, 0.0, -1.0 };
	p.vertices[4 + offs] = {  x,  y,  -z };  p.normals[4 + offs] = { 0.0, 0.0, -1.0 };
	p.vertices[5 + offs] = {  x, -y,  -z };  p.normals[5 + offs] = { 0.0, 0.0, -1.0 };
	
	offs = 6; 								// Back face
	p.vertices[0 + offs] = {  x, -y,  z };  p.normals[0 + offs] = { 0.0, 0.0, 1.0 };
	p.vertices[1 + offs] = {  x,  y,  z };  p.normals[1 + offs] = { 0.0, 0.0, 1.0 };
	p.vertices[2 + offs] = { -x,  y,  z };  p.normals[2 + offs] = { 0.0, 0.0, 1.0 };
	p.vertices[3 + offs] = {  x, -y,  z };  p.normals[3 + offs] = { 0.0, 0.0, 1.0 };
	p.vertices[4 + offs] = { -x,  y,  z };  p.normals[4 + offs] = { 0.0, 0.0, 1.0 };
	p.vertices[5 + offs] = { -x, -y,  z };  p.normals[5 + offs] = { 0.0, 0.0, 1.0 };
	
	offs = 12;								// Top face
	p.vertices[0 + offs] = { -x,  y, -z };  p.normals[0 + offs] = { 0.0, 1.0, 0.0 };
	p.vertices[1 + offs] = { -x,  y,  z };  p.normals[1 + offs] = { 0.0, 1.0, 0.0 };
	p.vertices[2 + offs] = {  x,  y,  z };  p.normals[2 + offs] = { 0.0, 1.0, 0.0 };
	p.vertices[3 + offs] = { -x,  y, -z };  p.normals[3 + offs] = { 0.0, 1.0, 0.0 };
	p.vertices[4 + offs] = {  x,  y,  z };  p.normals[4 + offs] = { 0.0, 1.0, 0.0 };
	p.vertices[5 + offs] = {  x,  y, -z };  p.normals[5 + offs] = { 0.0, 1.0, 0.0 };
	
	offs = 18;								// Bottom face
	p.vertices[0 + offs] = { -x, -y,  z };  p.normals[0 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[1 + offs] = { -x, -y, -z };  p.normals[1 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[2 + offs] = {  x, -y, -z };  p.normals[2 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[3 + offs] = { -x, -y,  z };  p.normals[3 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[4 + offs] = {  x, -y, -z };  p.normals[4 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[5 + offs] = {  x, -y,  z };  p.normals[5 + offs] = { 0.0, -1.0, 0.0 };
	
	offs = 24;								// Left face
	p.vertices[0 + offs] = { -x, -y,  z};   p.normals[0 + offs] = { -1.0, 0.0, 0.0 };
	p.vertices[1 + offs] = { -x,  y,  z };  p.normals[1 + offs] = { -1.0, 0.0, 0.0 };
	p.vertices[2 + offs] = { -x,  y, -z };  p.normals[2 + offs] = { -1.0, 0.0, 0.0 };
	p.vertices[3 + offs] = { -x, -y,  z };  p.normals[3 + offs] = { -1.0, 0.0, 0.0 };
	p.vertices[4 + offs] = { -x,  y, -z };  p.normals[4 + offs] = { -1.0, 0.0, 0.0 };
	p.vertices[5 + offs] = { -x, -y, -z };  p.normals[5 + offs] = { -1.0, 0.0, 0.0 };
	
	offs = 30;								// Right face
	p.vertices[0 + offs] = {  x, -y, -z};   p.normals[0 + offs] = { 1.0, 0.0, 0.0 };
	p.vertices[1 + offs] = {  x,  y, -z };  p.normals[1 + offs] = { 1.0, 0.0, 0.0 };
	p.vertices[2 + offs] = {  x,  y,  z };  p.normals[2 + offs] = { 1.0, 0.0, 0.0 };
	p.vertices[3 + offs] = {  x, -y, -z };  p.normals[3 + offs] = { 1.0, 0.0, 0.0 };
	p.vertices[4 + offs] = {  x,  y,  z };  p.normals[4 + offs] = { 1.0, 0.0, 0.0 };
	p.vertices[5 + offs] = {  x, -y,  z };  p.normals[5 + offs] = { 1.0, 0.0, 0.0 };
	
	return p;
}

Primitive pr_loadSquarePyramid(float w, float h) {
	Primitive p(18);
	
	int offs;
	w *= 0.5;
	
	v3f base_left_near  = { -w, 0.0, -w };
	v3f base_left_far   = { -w, 0.0,  w };
	v3f base_right_near = {  w, 0.0, -w };
	v3f base_right_far  = {  w, 0.0,  w };
	v3f apex = { 0.0, h, 0.0 };
	
	// t is a vector parallel to the normal. It is simply (-w/2, h).
	// n is (nHoriz, nVert) - t normalized.
	float modT = sqrtf(w*w + h*h);
	float nHoriz = w / modT;
	float nVert  = h / modT;
	
	// Base of pyramid
	offs = 0;
	p.vertices[0 + offs] = base_left_far;    p.normals[0 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[1 + offs] = base_left_near;   p.normals[1 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[2 + offs] = base_right_near;  p.normals[2 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[3 + offs] = base_left_far;    p.normals[3 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[4 + offs] = base_right_near;  p.normals[4 + offs] = { 0.0, -1.0, 0.0 };
	p.vertices[5 + offs] = base_right_far;   p.normals[5 + offs] = { 0.0, -1.0, 0.0 };
	
	// Near triangle
	offs = 6;
	p.vertices[0 + offs] = base_left_near;   p.normals[0 + offs] = { 0.0, nVert, -nHoriz };
	p.vertices[1 + offs] = apex;             p.normals[1 + offs] = { 0.0, nVert, -nHoriz };
	p.vertices[2 + offs] = base_right_near;  p.normals[2 + offs] = { 0.0, nVert, -nHoriz };
	
	// Far triangle
	offs = 9;
	p.vertices[0 + offs] = base_right_far;   p.normals[0 + offs] = { 0.0, nVert, nHoriz };
	p.vertices[1 + offs] = apex;             p.normals[1 + offs] = { 0.0, nVert, nHoriz };
	p.vertices[2 + offs] = base_left_far;    p.normals[2 + offs] = { 0.0, nVert, nHoriz };
	
	// Left triangle
	offs = 12;
	p.vertices[0 + offs] = base_left_far;    p.normals[0 + offs] = { -nHoriz, nVert, 0.0 };
	p.vertices[1 + offs] = apex;             p.normals[1 + offs] = { -nHoriz, nVert, 0.0 };
	p.vertices[2 + offs] = base_left_near;   p.normals[2 + offs] = { -nHoriz, nVert, 0.0 };
	
	// Right triangle
	offs = 15;
	p.vertices[0 + offs] = base_right_near;  p.normals[0 + offs] = { nHoriz, nVert, 0.0 };
	p.vertices[1 + offs] = apex;             p.normals[1 + offs] = { nHoriz, nVert, 0.0 };
	p.vertices[2 + offs] = base_right_far;   p.normals[2 + offs] = { nHoriz, nVert, 0.0 };
	
	return p;
}

