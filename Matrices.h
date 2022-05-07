//
//  MatrixFunctions.h
//  OpenGL3.2-SpinningCube
//
//  Created by Ben on 09/10/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#ifndef OpenGL3_2_SpinningCube_MatrixFunctions_h
#define OpenGL3_2_SpinningCube_MatrixFunctions_h

#define ONE_DEG_IN_RAD (M_PI/180)

#ifdef __cplusplus
extern "C" {
#endif

	void loadIdentityMatrix(float *mtx);

	void loadPerspectiveMatrix(float *mtx, float n, float f, float fov, float aspect);
	void loadOrthographicMatrix(float* mtx, float l, float r, float b, float t, float near, float far);
	
	void loadNormalMtxFromModelMtx(float *normalMtx, const float *modelMtx);
		// Normal mtx is inverse transpose of top left 3x3 of model mtx
	
	void multiplyMatrices(float* ret, const float* lhs, const float* rhs);
	
	void loadTranslationMatrix(float* mtx, float dx, float dy, float dz);
	void loadUniformScaleMatrix(float *mtx, float s);
	void loadScaleMatrix(float* mtx, float sx, float sy, float sz);
	void loadXRotationMatrix(float* mtx, float rad);
	void loadYRotationMatrix(float* mtx, float rad);
	void loadZRotationMatrix(float* mtx, float rad);
	void loadRotationMatrix(float* mtx, float deg, float xAxis, float yAxis, float zAxis);
	void loadRotationAboutCardinalAxesInTurn(float *mtx, float rx, float ry, float rz);
	
	// Apply: M = M x R
	//  i.e. when 'applied', transformations occur in standard order:
	//		 p -> BAp
	void applyTranslationMatrix(float *mtx, float dx, float dy, float dz);
	void applyUniformScaleMatrix(float *mtx, float s);
	void applyScaleMatrix(float *mtx, float sx, float sy, float sz);
	void applyXRotationMatrix(float *mtx, float deg);
	void applyYRotationMatrix(float *mtx, float deg);
	void applyZRotationMatrix(float *mtx, float deg);
	
	// Alter: M = R x M
	//  i.e. these functions add transformations in the order called:
	//       p -> ABp
	void matrixTranslate(float* mtx, float dx, float dy, float dz);
	void matrixScale(float* mtx, float sx, float sy, float sz);
	void matrixRotateX(float* mtx, float rad);
	void matrixRotateY(float* mtx, float rad);
	void matrixRotateZ(float* mtx, float rad);
	void matrixRotate(float* mtx, float rad, float xAxis, float yAxis, float zAxis);
	
	// Transpose B into A
	void transposeMatrix(float *A, const float *B);
	void mtx_transpose3x3(float *A, const float *B);
	
	// Invert B into A
	void invertMatrix(float *A, const float *B);
	void mtx_invert3x3(float *A, const float *B);
	
	// Get 3x3 from 4x4
	void mtx_reduce4x4to3x3(float *A, const float *B);

	
#ifdef __cplusplus
}
#endif

#endif
