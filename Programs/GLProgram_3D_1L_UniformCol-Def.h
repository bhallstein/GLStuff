//
//  GLProgram_3D_1L_UniformCol.h
//  IsometricOGLGame
//
//  Created by Ben on 01/02/2015.
//  Copyright (c) 2015 Ben. All rights reserved.
//

#ifndef __GLProgram_3D_1L_UniformCol_h
#define __GLProgram_3D_1L_UniformCol_h

#include "GLProgram.h"

class GLProgram_3D_1L_UniformCol_Def : public GLProgram {
public:
	GLProgram_3D_1L_UniformCol_Def();
	
	bool ok;
	
	struct Attribs {
		enum {
			VertPos,    Normal
		};
	};
	
	struct Uniforms {
		enum {
			MVPMtx,     ModelMtx,      NormalMtx,
			CamPos
		};
	};
	
	struct Attachments {
		enum {
			Normal
		};
	};
	
	unsigned int vao;
	struct {
		unsigned int
			vertexPos,
			normal;
	} buffers;
	
};


#endif
