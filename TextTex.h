//
//  TextTex.hpp
//  GLStuff_Renderers
//
//  Created by Ben on 18/07/2016.
//  Copyright © 2016 Ben. All rights reserved.
//

#ifndef TextTex_h
#define TextTex_h


#include "TexImage.h"
#include "CoordinateTypes.h"

#ifdef __cplusplus
extern "C" {
#endif
	
//struct Font {
//	
//};

struct TexImage get_textTex(const char *text,
							float fitWidth,
							float fitHeight,
							float font_size,
							struct col4 col,
							struct col4 backgroundCol);


#ifdef __cplusplus
}
#endif

#endif
