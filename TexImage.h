//
//  TexImage.h
//  OpenGL3.2-Mac
//
//  Created by Ben on 23/09/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#ifndef __TEX_IMAGE_H
#define __TEX_IMAGE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	void *data;
	float w, h;
} TexImage;

TexImage loadPngTexture(const char *fileName);
	// Pass the full file path
	// - get with bundledFilePath()

#ifdef __cplusplus
}
#endif

#endif
