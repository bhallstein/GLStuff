#ifndef TextTex_h
#define TextTex_h

#include "TexImage.hpp"
#include "CoordinateTypes.hpp"

struct TexImage get_textTex(const char *text,
														float fitWidth,
														float fitHeight,
														float font_size,
														struct col4 col,
														struct col4 backgroundCol);

#endif

