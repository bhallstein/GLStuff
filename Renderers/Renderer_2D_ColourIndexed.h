//
//  Renderer_2D_ColourIndexed
//  GLStuff
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLStuff_Renderer_2D_ColourIndexed
#define __GLStuff_Renderer_2D_ColourIndexed

class GLProg;


class Renderer_2D_ColourIndexed {
public:
	Renderer_2D_ColourIndexed(bool dither);
	~Renderer_2D_ColourIndexed();
	
	bool setUp();
	void render(float *ortho_mtx);
	
	struct Attribs {
		enum { VertPos, Colour };
	};
	
	struct Uniforms {
		enum { OrthoMatrix, Sampler_Dithering };
	};
	
	struct {
		unsigned int vertexPos, colour;
	} buffers;
	
	int n_vertices;
	
private:
	GLProg *prog;
	unsigned int vao;
	
	bool dither;
	unsigned int tex_dither;
};

#endif
