#ifndef TextTex_h
#define TextTex_h

#include "vectors.hpp"
#include "TexImage.hpp"

struct TexImage get_textTex(const char *text,
														float fitWidth,
														float fitHeight,
														float font_size,
														v4 col,
														v4 backgroundCol);

#endif
