//
//  ShaderStr.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 25/09/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#ifndef __SHADER_STR_H
#define __SHADER_STR_H

#ifdef __cplusplus
extern "C" {
#endif

char* loadShaderString(const char *file);
	// Takes the full file path
	// - must get this with bundledFilePath()

	
#ifdef __cplusplus
}
#endif
	
#endif
