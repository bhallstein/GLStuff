//
//  GLProgram_2D_ColourIndexed.cpp
//  OpenGL3_2_Render_to_Tex
//
//  Created by Ben on 30/06/2014.
//  Copyright (c) 2014 Ben. All rights reserved.
//

#include "GLProgram_3D_ReflectionMapped.h"
#include "GLHelpers.h"


enum {
	_3DREFLMAP_INDEX_OF_VERT_POSITION_ATTRIB,
	_3DREFLMAP_INDEX_OF_NORMAL_ATTRIB
};

GLProgram_3D_ReflectionMapped::GLProgram_3D_ReflectionMapped() :
	GLProgram("3D_ReflectionMapped_v.sh", "3D_ReflectionMapped_f.sh")
{
	
}

void GLProgram_3D_ReflectionMapped::setAttribLocations() {
	prog_setAttribLocation(programID, _3DREFLMAP_INDEX_OF_VERT_POSITION_ATTRIB, "inVPos");
	prog_setAttribLocation(programID, _3DREFLMAP_INDEX_OF_NORMAL_ATTRIB, "inVNormal");
}
bool GLProgram_3D_ReflectionMapped::getUniformLocations() {
	// MVP matrix uniform
	uLoc_mvpMatrix = prog_uniformLocation(programID, "mvpMtx");
	if (uLoc_mvpMatrix == -1) return false;
	
	// Model matrix uniform
	uLoc_modelMatrix = prog_uniformLocation(programID, "modelMtx");
	if (uLoc_mvpMatrix == -1) return false;
	
	// Normal matrix uniform
	uLoc_normalMatrix = prog_uniformLocation(programID, "normalMtx");
	if (uLoc_normalMatrix == -1) return false;
	
	// Light colour uniform
//	uLoc_lightCol = prog_uniformLocation(programID, "uLightCol");
//	if (uLoc_lightCol == -1) return false;
	
	// Light direction uniform
	uLoc_lightVec = prog_uniformLocation(programID, "uLightVector");
	if (uLoc_lightVec == -1) return false;
	
	// Light components uniform
	uLoc_lightProp = prog_uniformLocation(programID, "uLightProperties");
	if (uLoc_lightProp == -1) return false;
	
	// Camera position uniform
	uLoc_camPosition = prog_uniformLocation(programID, "uCamPosition");
	if (uLoc_camPosition == -1) return false;
	
	// Reflection to light ratio uniform
	uLoc_reflToLightRatio = prog_uniformLocation(programID, "reflToLightRatio");
	if (uLoc_reflToLightRatio == -1) return false;
	
	// Cube map sampler uniform (tex unit)
	uLoc_cmSampler = prog_uniformLocation(programID, "uCubeMapTex");
	if (uLoc_cmSampler == -1) return false;
	
	return true;
}

void GLProgram_3D_ReflectionMapped::setBuffersForAttribs(unsigned int vert_vboID, unsigned int norm_vboID) {
	vboID_vertexPos = vert_vboID;
	vboID_normals = norm_vboID;
}

void GLProgram_3D_ReflectionMapped::setUpAttribsForDrawing() {
	prog_setAttribToUseVBO(_3DREFLMAP_INDEX_OF_VERT_POSITION_ATTRIB, vboID_vertexPos, 3, ATTRTYPE_FLOAT);
	prog_setAttribToUseVBO(_3DREFLMAP_INDEX_OF_NORMAL_ATTRIB, vboID_normals, 3, ATTRTYPE_FLOAT);
}

void GLProgram_3D_ReflectionMapped::setMVPMatrix(float *matr) {
	prog_setUniformValue_Mat4(uLoc_mvpMatrix, matr);
}
void GLProgram_3D_ReflectionMapped::setModelMatrix(float *matr) {
	prog_setUniformValue_Mat4(uLoc_modelMatrix, matr);
}
void GLProgram_3D_ReflectionMapped::setNormalMatrix(float *matr) {
	prog_setUniformValue_Mat3(uLoc_normalMatrix, matr);
}

void GLProgram_3D_ReflectionMapped::setLightVector(float *x) {
	prog_setUniformValue_Vec3(uLoc_lightVec, x);
}
void GLProgram_3D_ReflectionMapped::setLightComponents(float *x) {
	prog_setUniformValue_Vec3(uLoc_lightProp, x);
}

void GLProgram_3D_ReflectionMapped::setCameraPosition(float *x) {
	prog_setUniformValue_Vec3(uLoc_camPosition, x);
}

void GLProgram_3D_ReflectionMapped::setReflectionPercent(float x) {
	prog_setUniformValue_Float(uLoc_reflToLightRatio, x);
}
