//
//  Renderer_PixelCol.h
//
//  Use Renderer_2D_ColourIndexed but take pixel values and a single colour
//
//  Created by Ben on 02/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLStuff_Renderer_PixelCol
#define __GLStuff_Renderer_PixelCol

#include "CoordinateTypes.h"
#include "Renderer_2D_ColourIndexed.h"

class GLProg;


class Renderer_PixelCol {
public:
	Renderer_PixelCol();
	~Renderer_PixelCol();

	bool setUp();
	void setRect(const v2 &pos, const v2 &size);
	void setColour(const col4 &);
	void render(v2 win_size);

private:
	Renderer_2D_ColourIndexed *rend;
	v2 _pos, _size;
	col4 _col;
};

#endif

