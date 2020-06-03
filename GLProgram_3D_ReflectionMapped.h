//
//  GLProgram_2D_Textured.h
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#ifndef __GLPROGRAM_2D_Refl_Mapped_H
#define __GLPROGRAM_2D_Refl_Mapped_H

#include "GLProgram.h"

class GLProgram_3D_ReflectionMapped : public GLProgram {
public:
	GLProgram_3D_ReflectionMapped();
	
	void setBuffersForAttribs(unsigned int vert_vboID, unsigned int norm_vboID);
	void setUpAttribsForDrawing();
	
	void setMVPMatrix(float *);
	void setModelMatrix(float *);
	void setNormalMatrix(float *);
	
	void setLightVector(float *);
	void setLightComponents(float *);
		// Ambient, Diffuse, Specular
	
	void setCameraPosition(float *);
	
	void setReflectionPercent(float);
		// Percent of colour contributed by reflections, rather than lighting
	
protected:
	void setAttribLocations();
	bool getUniformLocations();
	
	int uLoc_mvpMatrix;
	int uLoc_modelMatrix;
	int uLoc_normalMatrix;
	
//	int uLoc_lightCol;
	int uLoc_lightVec;
	int uLoc_lightProp;

	int uLoc_camPosition;
	
	int uLoc_reflToLightRatio;
	int uLoc_cmSampler;
	
	int vboID_vertexPos;
	int vboID_normals;
};

#endif