//
//  MatrixFunctions.c
//  OpenGL3.2-SpinningCube
//
//  Created by Ben on 09/10/2013.
//  Copyright (c) 2013 Ben. All rights reserved.
//

#include <stdio.h>
#include "math.h"
#include "Matrices.h"

void loadIdentityMatrix(float *mtx) {
	for (int i=0; i < 4; ++i)
		for (int j=0; j < 4; ++j)
			mtx[j*4 + i] = (i == j ? 1 : 0);
}

void loadPerspectiveMatrix(float *mtx, float n, float f, float fov, float aspect) {
	float t = n * tan(0.5 * fov * ONE_DEG_IN_RAD);
	float b = -t;
	
	float r = t * aspect;
	float l = -r;
	
	// Set matrix fields
	mtx[0] = 2*n/(r-l);		mtx[4] = 0.0;			mtx[8]  = (r+l)/(r-l);		mtx[12] = 0.0;
	mtx[1] = 0.0;			mtx[5] = 2*n/(t-b);		mtx[9]  = (t+b)/(t-b); 		mtx[13] = 0.0;
	mtx[2] = 0.0;			mtx[6] = 0.0;			mtx[10] = (n+f)/(n-f); 		mtx[14] = 2*n*f/(n-f);
	mtx[3] = 0.0;			mtx[7] = 0.0;			mtx[11] = -1.0;        		mtx[15] = 0.0;
}

void loadOrthographicMatrix(float* mtx,
							float left, float right,
							float bottom, float top,
							float nearZ, float farZ) {
	mtx[ 0] = 2.0f / (right - left);
	mtx[ 1] = 0.0;
	mtx[ 2] = 0.0;
	mtx[ 3] = 0.0;
	
	mtx[ 4] = 0.0;
	mtx[ 5] = 2.0f / (top - bottom);
	mtx[ 6] = 0.0;
	mtx[ 7] = 0.0;
	
	mtx[ 8] = 0.0;
	mtx[ 9] = 0.0;
	mtx[10] = -2.0f / (farZ - nearZ);
	mtx[11] = 0.0;
	
	mtx[12] = -(right + left) / (right - left);
	mtx[13] = -(top + bottom) / (top - bottom);
	mtx[14] = -(farZ + nearZ) / (farZ - nearZ);
	mtx[15] = 1.0f;
}

void loadNormalMtxFromModelMtx(float *normalMtx, const float *modelMtx) {
	float tempMtx[9];
	mtx_reduce4x4to3x3(normalMtx, modelMtx);
	transposeMatrix(tempMtx, normalMtx);
	invertMatrix(normalMtx, tempMtx);
}

void multiplyMatrices(float* ret, const float* lhs, const float* rhs) {
	// [ 0 4  8 12 ]   [ 0 4  8 12 ]
	// [ 1 5  9 13 ] x [ 1 5  9 13 ]
	// [ 2 6 10 14 ]   [ 2 6 10 14 ]
	// [ 3 7 11 15 ]   [ 3 7 11 15 ]
	ret[ 0] = lhs[ 0]*rhs[ 0] + lhs[ 4]*rhs[ 1] + lhs[ 8]*rhs[ 2] + lhs[12]*rhs[ 3];
	ret[ 1] = lhs[ 1]*rhs[ 0] + lhs[ 5]*rhs[ 1] + lhs[ 9]*rhs[ 2] + lhs[13]*rhs[ 3];
	ret[ 2] = lhs[ 2]*rhs[ 0] + lhs[ 6]*rhs[ 1] + lhs[10]*rhs[ 2] + lhs[14]*rhs[ 3];
	ret[ 3] = lhs[ 3]*rhs[ 0] + lhs[ 7]*rhs[ 1] + lhs[11]*rhs[ 2] + lhs[15]*rhs[ 3];
	
	ret[ 4] = lhs[ 0]*rhs[ 4] + lhs[ 4]*rhs[ 5] + lhs[ 8]*rhs[ 6] + lhs[12]*rhs[ 7];
	ret[ 5] = lhs[ 1]*rhs[ 4] + lhs[ 5]*rhs[ 5] + lhs[ 9]*rhs[ 6] + lhs[13]*rhs[ 7];
	ret[ 6] = lhs[ 2]*rhs[ 4] + lhs[ 6]*rhs[ 5] + lhs[10]*rhs[ 6] + lhs[14]*rhs[ 7];
	ret[ 7] = lhs[ 3]*rhs[ 4] + lhs[ 7]*rhs[ 5] + lhs[11]*rhs[ 6] + lhs[15]*rhs[ 7];
	
	ret[ 8] = lhs[ 0]*rhs[ 8] + lhs[ 4]*rhs[ 9] + lhs[ 8]*rhs[10] + lhs[12]*rhs[11];
	ret[ 9] = lhs[ 1]*rhs[ 8] + lhs[ 5]*rhs[ 9] + lhs[ 9]*rhs[10] + lhs[13]*rhs[11];
	ret[10] = lhs[ 2]*rhs[ 8] + lhs[ 6]*rhs[ 9] + lhs[10]*rhs[10] + lhs[14]*rhs[11];
	ret[11] = lhs[ 3]*rhs[ 8] + lhs[ 7]*rhs[ 9] + lhs[11]*rhs[10] + lhs[15]*rhs[11];
	
	ret[12] = lhs[ 0]*rhs[12] + lhs[ 4]*rhs[13] + lhs[ 8]*rhs[14] + lhs[12]*rhs[15];
	ret[13] = lhs[ 1]*rhs[12] + lhs[ 5]*rhs[13] + lhs[ 9]*rhs[14] + lhs[13]*rhs[15];
	ret[14] = lhs[ 2]*rhs[12] + lhs[ 6]*rhs[13] + lhs[10]*rhs[14] + lhs[14]*rhs[15];
	ret[15] = lhs[ 3]*rhs[12] + lhs[ 7]*rhs[13] + lhs[11]*rhs[14] + lhs[15]*rhs[15];
}

void loadTranslationMatrix(float* mtx, float xTrans, float yTrans, float zTrans) {
	mtx[ 0] = mtx[ 5] = mtx[10] = mtx[15] = 1.0f;
	
	mtx[ 1] = mtx[ 2] = mtx[ 3] = mtx[ 4] =
	mtx[ 6] = mtx[ 7] = mtx[ 8] = mtx[ 9] =
	mtx[11] = 0.0;
	
	mtx[12] = xTrans;
	mtx[13] = yTrans;
	mtx[14] = zTrans;
}

void loadUniformScaleMatrix(float *mtx, float s) {
	loadScaleMatrix(mtx, s, s, s);
}

void loadScaleMatrix(float* mtx, float xScale, float yScale, float zScale) {
	mtx[ 0] = xScale;
	mtx[ 5] = yScale;
	mtx[10] = zScale;
	mtx[15] = 1.0f;
	
	mtx[ 1] = mtx[ 2] = mtx[ 3] = mtx[ 4] =
	mtx[ 6] = mtx[ 7] = mtx[ 8] = mtx[ 9] =
	mtx[11] = mtx[12] = mtx[13] = mtx[14] = 0.0;
}

void loadXRotationMatrix(float* mtx, float rad) {
	// [ 0 4      8 12 ]
	// [ 1 cos -sin 13 ]
	// [ 2 sin cos  14 ]
	// [ 3 7     11 15 ]
	mtx[10] = mtx[ 5] = cosf(rad);
	mtx[ 6] = sinf(rad);
	mtx[ 9] = -mtx[ 6];
	
	mtx[ 0] = mtx[15] = 1.0f;
	
	mtx[ 1] = mtx[ 2] = mtx[ 3] = mtx[ 4] =
	mtx[ 7] = mtx[ 8] = mtx[11] = mtx[12] =
	mtx[13] = mtx[14] = 0.0;
}

void loadYRotationMatrix(float* mtx, float rad) {
	// [ cos 4  -sin 12 ]
	// [ 1   5   9   13 ]
	// [ sin 6  cos  14 ]
	// [ 3   7  11   15 ]
	
	mtx[ 0] = mtx[10] = cosf(rad);
	mtx[ 2] = sinf(rad);
	mtx[ 8] = -mtx[2];
	
	mtx[ 5] = mtx[15] = 1.0;
	
	mtx[ 1] = mtx[ 3] = mtx[ 4] = mtx[ 6] =
	mtx[ 7] = mtx[ 9] = mtx[11] = mtx[12] =
	mtx[13] = mtx[14] = 0.0;
}

void loadZRotationMatrix(float* mtx, float rad) {
	// [ cos -sin 8 12 ]
	// [ sin cos  9 13 ]
	// [ 2   6   10 14 ]
	// [ 3   7   11 15 ]
	
	mtx[ 0] = mtx[ 5] = cosf(rad);
	mtx[ 1] = sinf(rad);
	mtx[ 4] = -mtx[1];
	
	mtx[10] = mtx[15] = 1.0;
	
	mtx[ 2] = mtx[ 3] = mtx[ 6] = mtx[ 7] =
	mtx[ 8] = mtx[ 9] = mtx[11] = mtx[12] =
	mtx[13] = mtx[14] = 0.0;
}

void loadRotationMatrix(float* mtx, float deg, float xAxis, float yAxis, float zAxis) {
	float rad = deg * M_PI/180.0f;
	
	float sin_a = sinf(rad);
	float cos_a = cosf(rad);
	
	// Calculate coeffs.  No need to check for zero magnitude because we wouldn't be here.
	float magnitude = sqrtf(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);
	
	float p = 1.0f / magnitude;
	float cos_am = 1.0f - cos_a;
	
	float xp = xAxis * p;
	float yp = yAxis * p;
	float zp = zAxis * p;
	
	float xx = xp * xp;
	float yy = yp * yp;
	float zz = zp * zp;
	
	float xy = xp * yp * cos_am;
	float yz = yp * zp * cos_am;
	float zx = zp * xp * cos_am;
	
	xp *= sin_a;
	yp *= sin_a;
	zp *= sin_a;
	
	// Load coefs
	float m0  = xx + cos_a * (1.0f - xx);
	float m1  = xy + zp;
	float m2  = zx - yp;
	float m4  = xy - zp;
	float m5  = yy + cos_a * (1.0f - yy);
	float m6  = yz + xp;
	float m8  = zx + yp;
	float m9  = yz - xp;
	float m10 = zz + cos_a * (1.0f - zz);
	
	// Apply rotation
	float c1 = mtx[0];
	float c2 = mtx[4];
	float c3 = mtx[8];
	mtx[0]  = c1 * m0 + c2 * m1 + c3 * m2;
	mtx[4]  = c1 * m4 + c2 * m5 + c3 * m6;
	mtx[8]  = c1 * m8 + c2 * m9 + c3 * m10;
	
	c1 = mtx[1];
	c2 = mtx[5];
	c3 = mtx[9];
	mtx[1]  = c1 * m0 + c2 * m1 + c3 * m2;
	mtx[5]  = c1 * m4 + c2 * m5 + c3 * m6;
	mtx[9]  = c1 * m8 + c2 * m9 + c3 * m10;
	
	c1 = mtx[2];
	c2 = mtx[6];
	c3 = mtx[10];
	mtx[2]  = c1 * m0 + c2 * m1 + c3 * m2;
	mtx[6]  = c1 * m4 + c2 * m5 + c3 * m6;
	mtx[10] = c1 * m8 + c2 * m9 + c3 * m10;
	
	c1 = mtx[3];
	c2 = mtx[7];
	c3 = mtx[11];
	mtx[3]  = c1 * m0 + c2 * m1 + c3 * m2;
	mtx[7]  = c1 * m4 + c2 * m5 + c3 * m6;
	mtx[11] = c1 * m8 + c2 * m9 + c3 * m10;
	
	mtx[12] = mtx[13] = mtx[14] = 0.0;
	mtx[15] = 1.0f;
}

void loadRotationAboutCardinalAxesInTurn(float *mtx, float rx, float ry, float rz) {
	// Perform a rotation first about x, then about y, then about z
	// (This is a mildly daft thing to do.)
	// [ 0 4  8 12 ]
	// [ 1 5  9 13 ]
	// [ 2 6 10 14 ]
	// [ 3 7 11 15 ]
	rx *= ONE_DEG_IN_RAD;
	ry *= ONE_DEG_IN_RAD;
	rz *= ONE_DEG_IN_RAD;
	
	float sinx = sin(rx), cosx = cos(rx);
	float siny = sin(ry), cosy = cos(ry);
	float sinz = sin(rz), cosz = cos(rz);
	
	mtx[0] = cosy*cosz;
	mtx[4] = sinx*siny*cosz - cosx*sinz;
	mtx[8] = cosx*siny*cosz + sinx*sinz;
	mtx[1] = cosy*sinz;
	mtx[5] = sinx*siny*sinz + cosx*cosz;
	mtx[9] = cosx*siny*sinz - sinx*cosz;
	mtx[2] = -siny;
	mtx[6] = sinx*cosy;
	mtx[10] = cosx*cosy;
	
	mtx[12] = mtx[13] = mtx[14] = mtx[3] = mtx[7] = mtx[11] = 0.0;
	mtx[15] = 1.0;
}

void applyTranslationMatrix(float* mtx, float xTrans, float yTrans, float zTrans) {
	// [ 0 4  8 12 ]   [ 1 0 0 x ]
	// [ 1 5  9 13 ] x [ 0 1 0 y ]
	// [ 2 6 10 14 ]   [ 0 0 1 z ]
	// [ 3 7 11 15 ]   [ 0 0 0 1 ]
	
	mtx[12] += mtx[0]*xTrans + mtx[4]*yTrans + mtx[ 8]*zTrans;
	mtx[13] += mtx[1]*xTrans + mtx[5]*yTrans + mtx[ 9]*zTrans;
	mtx[14] += mtx[2]*xTrans + mtx[6]*yTrans + mtx[10]*zTrans;
}

void applyUniformScaleMatrix(float *mtx, float s) {
	applyScaleMatrix(mtx, s, s, s);
}

void applyScaleMatrix(float* mtx, float xScale, float yScale, float zScale) {
    // [ 0 4  8 12 ]   [ x 0 0 0 ]
    // [ 1 5  9 13 ] x [ 0 y 0 0 ]
    // [ 2 6 10 14 ]   [ 0 0 z 0 ]
    // [ 3 7 11 15 ]   [ 0 0 0 1 ]
	
	mtx[ 0] *= xScale;
	mtx[ 4] *= yScale;
	mtx[ 8] *= zScale;
	
	mtx[ 1] *= xScale;
	mtx[ 5] *= yScale;
	mtx[ 9] *= zScale;
	
	mtx[ 2] *= xScale;
	mtx[ 6] *= yScale;
	mtx[10] *= zScale;
	
	mtx[ 3] *= xScale;
	mtx[ 7] *= yScale;
	mtx[11] *= xScale;
}

void applyXRotationMatrix(float* mtx, float deg) {
	// [ 0 4  8 12 ]   [ 1  0    0  0 ]
	// [ 1 5  9 13 ] x [ 0 cos -sin 0 ]
	// [ 2 6 10 14 ]   [ 0 sin  cos 0 ]
	// [ 3 7 11 15 ]   [ 0  0    0  1 ]
	
	float rad = deg * (M_PI/180.0f);
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx04 = mtx[4];
	float mtx05 = mtx[5];
	float mtx06 = mtx[6];
	float mtx07 = mtx[7];
	
	mtx[ 4] = mtx[ 8]*sinrad + mtx04*cosrad;
	mtx[ 8] = mtx[ 8]*cosrad - mtx04*sinrad;
	
	mtx[ 5] = mtx[ 9]*sinrad + mtx05*cosrad;
	mtx[ 9] = mtx[ 9]*cosrad - mtx05*sinrad;
	
	mtx[ 6] = mtx[10]*sinrad + mtx06*cosrad;
	mtx[10] = mtx[10]*cosrad - mtx06*sinrad;
	
	mtx[ 7] = mtx[11]*sinrad + mtx07*cosrad;
	mtx[11] = mtx[11]*cosrad - mtx07*sinrad;
}


void applyYRotationMatrix(float* mtx, float deg) {
	// [ 0 4  8 12 ]   [ cos 0  -sin 0 ]
	// [ 1 5  9 13 ] x [ 0   1  0    0 ]
	// [ 2 6 10 14 ]   [ sin 0  cos  0 ]
	// [ 3 7 11 15 ]   [ 0   0  0    1 ]
	
	float rad = deg * (M_PI/180.0f);
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[0];
	float mtx01 = mtx[1];
	float mtx02 = mtx[2];
	float mtx03 = mtx[3];
	
	mtx[ 0] = mtx[ 8]*sinrad + mtx00*cosrad;
	mtx[ 8] = mtx[ 8]*cosrad - mtx00*sinrad;
	
	mtx[ 1] = mtx[ 9]*sinrad + mtx01*cosrad;
	mtx[ 9] = mtx[ 9]*cosrad - mtx01*sinrad;
	
	mtx[ 2] = mtx[10]*sinrad + mtx02*cosrad;
	mtx[10] = mtx[10]*cosrad - mtx02*sinrad;
	
	mtx[ 3] = mtx[11]*sinrad + mtx03*cosrad;
	mtx[11] = mtx[11]*cosrad - mtx03*sinrad;
}


void applyZRotationMatrix(float* mtx, float deg) {
	// [ 0 4  8 12 ]   [ cos -sin 0  0 ]
	// [ 1 5  9 13 ] x [ sin cos  0  0 ]
	// [ 2 6 10 14 ]   [ 0   0    1  0 ]
	// [ 3 7 11 15 ]   [ 0   0    0  1 ]
	
	float rad = deg * (M_PI/180.0f);
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[0];
	float mtx01 = mtx[1];
	float mtx02 = mtx[2];
	float mtx03 = mtx[3];
	
	mtx[ 0] = mtx[ 4]*sinrad + mtx00*cosrad;
	mtx[ 4] = mtx[ 4]*cosrad - mtx00*sinrad;
	
	mtx[ 1] = mtx[ 5]*sinrad + mtx01*cosrad;
	mtx[ 5] = mtx[ 5]*cosrad - mtx01*sinrad;
	
	mtx[ 2] = mtx[ 6]*sinrad + mtx02*cosrad;
	mtx[ 6] = mtx[ 6]*cosrad - mtx02*sinrad;
	
	mtx[ 3] = mtx[ 7]*sinrad + mtx03*cosrad;
	mtx[ 7] = mtx[ 7]*cosrad - mtx03*sinrad;
}

void mtxRotateApply(float* mtx, float deg, float xAxis, float yAxis, float zAxis)
{
	if(yAxis == 0.0f && zAxis == 0.0f)
		applyXRotationMatrix(mtx, deg);
	else if(xAxis == 0.0f && zAxis == 0.0f)
		applyYRotationMatrix(mtx, deg);
	else if(xAxis == 0.0f && yAxis == 0.0f)
		applyZRotationMatrix(mtx, deg);
	else {
		float rad = deg * M_PI/180.0f;
		
		float sin_a = sinf(rad);
		float cos_a = cosf(rad);
		
		// Calculate coeffs.  No need to check for zero magnitude because we wouldn't be here.
		float magnitude = sqrtf(xAxis * xAxis + yAxis * yAxis + zAxis * zAxis);
		
		float p = 1.0f / magnitude;
		float cos_am = 1.0f - cos_a;
		
		float xp = xAxis * p;
		float yp = yAxis * p;
		float zp = zAxis * p;
		
		float xx = xp * xp;
		float yy = yp * yp;
		float zz = zp * zp;
		
		float xy = xp * yp * cos_am;
		float yz = yp * zp * cos_am;
		float zx = zp * xp * cos_am;
		
		xp *= sin_a;
		yp *= sin_a;
		zp *= sin_a;
		
		// Load coefs
		float m0  = xx + cos_a * (1.0f - xx);
		float m1  = xy + zp;
		float m2  = zx - yp;
		float m4  = xy - zp;
		float m5  = yy + cos_a * (1.0f - yy);
		float m6  = yz + xp;
		float m8  = zx + yp;
		float m9  = yz - xp;
		float m10 = zz + cos_a * (1.0f - zz);
		
		// Apply rotation
		float c1 = mtx[0];
		float c2 = mtx[4];
		float c3 = mtx[8];
		mtx[0]  = c1 * m0 + c2 * m1 + c3 * m2;
		mtx[4]  = c1 * m4 + c2 * m5 + c3 * m6;
		mtx[8]  = c1 * m8 + c2 * m9 + c3 * m10;
		
		c1 = mtx[1];
		c2 = mtx[5];
		c3 = mtx[9];
		mtx[1]  = c1 * m0 + c2 * m1 + c3 * m2;
		mtx[5]  = c1 * m4 + c2 * m5 + c3 * m6;
		mtx[9]  = c1 * m8 + c2 * m9 + c3 * m10;
		
		c1 = mtx[2];
		c2 = mtx[6];
		c3 = mtx[10];
		mtx[2]  = c1 * m0 + c2 * m1 + c3 * m2;
		mtx[6]  = c1 * m4 + c2 * m5 + c3 * m6;
		mtx[10] = c1 * m8 + c2 * m9 + c3 * m10;
		
		c1 = mtx[3];
		c2 = mtx[7];
		c3 = mtx[11];
		mtx[3]  = c1 * m0 + c2 * m1 + c3 * m2;
		mtx[7]  = c1 * m4 + c2 * m5 + c3 * m6;
		mtx[11] = c1 * m8 + c2 * m9 + c3 * m10;
	}
}

void matrixTranslate(float* mtx, float xTrans, float yTrans, float zTrans) {
	// [ 1 0 0 x ]   [ 0 4  8 12 ]
	// [ 0 1 0 y ] x [ 1 5  9 13 ]
	// [ 0 0 1 z ]   [ 2 6 10 14 ]
	// [ 0 0 0 1 ]   [ 3 7 11 15 ]
	
	mtx[ 0] += xTrans * mtx[ 3];
	mtx[ 1] += yTrans * mtx[ 3];
	mtx[ 2] += zTrans * mtx[ 3];
	
	mtx[ 4] += xTrans * mtx[ 7];
	mtx[ 5] += yTrans * mtx[ 7];
	mtx[ 6] += zTrans * mtx[ 7];
	
	mtx[ 8] += xTrans * mtx[11];
	mtx[ 9] += yTrans * mtx[11];
	mtx[10] += zTrans * mtx[11];
	
	mtx[12] += xTrans * mtx[15];
	mtx[13] += yTrans * mtx[15];
	mtx[14] += zTrans * mtx[15];
}

void matrixScale(float* mtx, float xScale, float yScale, float zScale) {
    // [ x 0 0 0 ]   [ 0 4  8 12 ]
    // [ 0 y 0 0 ] x [ 1 5  9 13 ]
    // [ 0 0 z 0 ]   [ 2 6 10 14 ]
    // [ 0 0 0 1 ]   [ 3 7 11 15 ]
	
	mtx[ 0] *= xScale;
	mtx[ 4] *= xScale;
	mtx[ 8] *= xScale;
	mtx[12] *= xScale;
	
	mtx[ 1] *= yScale;
	mtx[ 5] *= yScale;
	mtx[ 9] *= yScale;
	mtx[13] *= yScale;
	
	mtx[ 2] *= zScale;
	mtx[ 6] *= zScale;
	mtx[10] *= zScale;
	mtx[14] *= zScale;
}

void matrixRotateX(float* mtx, float rad) {
	// [ 1  0    0  0 ]   [ 0 4  8 12 ]
	// [ 0 cos -sin 0 ] x [ 1 5  9 13 ]
	// [ 0 sin  cos 0 ]   [ 2 6 10 14 ]
	// [ 0  0    0  1 ]   [ 3 7 11 15 ]
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx01 = mtx[ 1];
	float mtx05 = mtx[ 5];
	float mtx09 = mtx[ 9];
	float mtx13 = mtx[13];
	
	mtx[ 1] = cosrad*mtx01 - sinrad*mtx[ 2];
	mtx[ 2] = sinrad*mtx01 + cosrad*mtx[ 2];
	
	mtx[ 5] = cosrad*mtx05 - sinrad*mtx[ 6];
	mtx[ 6] = sinrad*mtx05 + cosrad*mtx[ 6];
	
	mtx[ 9] = cosrad*mtx09 - sinrad*mtx[10];
	mtx[10] = sinrad*mtx09 + cosrad*mtx[10];
	
	mtx[13] = cosrad*mtx13 - sinrad*mtx[14];
	mtx[14] = sinrad*mtx13 + cosrad*mtx[14];
}


void matrixRotateY(float* mtx, float rad)
{
	// [ cos 0  -sin 0 ]   [ 0 4  8 12 ]
	// [ 0   1  0    0 ] x [ 1 5  9 13 ]
	// [ sin 0  cos  0 ]   [ 2 6 10 14 ]
	// [ 0   0  0    1 ]   [ 3 7 11 15 ]
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[ 0];
	float mtx04 = mtx[ 4];
	float mtx08 = mtx[ 8];
	float mtx12 = mtx[12];
	
	mtx[ 0] = cosrad*mtx00 - sinrad*mtx[ 2];
	mtx[ 2] = sinrad*mtx00 + cosrad*mtx[ 2];
	
	mtx[ 4] = cosrad*mtx04 - sinrad*mtx[ 6];
	mtx[ 6] = sinrad*mtx04 + cosrad*mtx[ 6];
	
	mtx[ 8] = cosrad*mtx08 - sinrad*mtx[10];
	mtx[10] = sinrad*mtx08 + cosrad*mtx[10];
	
	mtx[12] = cosrad*mtx12 - sinrad*mtx[14];
	mtx[14] = sinrad*mtx12 + cosrad*mtx[14];
}


void matrixRotateZ(float* mtx, float rad) {
	// [ cos -sin 0  0 ]   [ 0 4  8 12 ]
	// [ sin cos  0  0 ] x [ 1 5  9 13 ]
	// [ 0   0    1  0 ]   [ 2 6 10 14 ]
	// [ 0   0    0  1 ]   [ 3 7 11 15 ]
	
	float cosrad = cosf(rad);
	float sinrad = sinf(rad);
	
	float mtx00 = mtx[ 0];
	float mtx04 = mtx[ 4];
	float mtx08 = mtx[ 8];
	float mtx12 = mtx[12];
	
	mtx[ 0] = cosrad*mtx00 - sinrad*mtx[ 1];
	mtx[ 1] = sinrad*mtx00 + cosrad*mtx[ 1];
	
	mtx[ 4] = cosrad*mtx04 - sinrad*mtx[ 5];
	mtx[ 5] = sinrad*mtx04 + cosrad*mtx[ 5];
	
	mtx[ 8] = cosrad*mtx08 - sinrad*mtx[ 9];
	mtx[ 9] = sinrad*mtx08 + cosrad*mtx[ 9];
	
	mtx[12] = cosrad*mtx12 - sinrad*mtx[13];
	mtx[13] = sinrad*mtx12 + cosrad*mtx[13];
}

void matrixRotate(float* mtx, float rad, float xAxis, float yAxis, float zAxis) {
	float rotMtx[16];
	loadRotationMatrix(rotMtx, rad, xAxis, yAxis, zAxis);
	multiplyMatrices(mtx, rotMtx, mtx);
}


void transposeMatrix(float *mtx, const float *src) {
	//Use a temp to swap in case mtx == src
	
	float tmp;
	mtx[0]  = src[0];
	mtx[5]  = src[5];
	mtx[10] = src[10];
	mtx[15] = src[15];
	
	tmp = src[4];
	mtx[4]  = src[1];
	mtx[1]  = tmp;
	
	tmp = src[8];
	mtx[8]  = src[2];
	mtx[2] = tmp;
	
	tmp = src[12];
	mtx[12] = src[3];
	mtx[3]  = tmp;
	
	tmp = src[9];
	mtx[9]  = src[6];
	mtx[6]  = tmp;
	
	tmp = src[13];
	mtx[13] = src[7];
	mtx[ 7] = tmp;
	
	tmp = src[14];
	mtx[14] = src[11];
	mtx[11] = tmp;
}
void mtx_transpose3x3(float *a, const float *b) {
	// [ 0  3  6 ]      [ 0  1  2 ]
	// [ 1  4  7 ]  ->  [ 3  4  5 ]
	// [ 2  5  8 ]      [ 6  7  8 ]
	a[0] = b[0];  a[3] = b[1];  a[6] = b[2];
	a[1] = b[3];  a[4] = b[4];  a[7] = b[5];
	a[2] = b[6];  a[5] = b[7];  a[8] = b[8];
}

void invertMatrix(float *invOut, const float *m) {
    float inv[16], det;
    int i;
	
    inv[0] = m[5]  * m[10] * m[15] -
	m[5]  * m[11] * m[14] -
	m[9]  * m[6]  * m[15] +
	m[9]  * m[7]  * m[14] +
	m[13] * m[6]  * m[11] -
	m[13] * m[7]  * m[10];
	
    inv[4] = -m[4]  * m[10] * m[15] +
	m[4]  * m[11] * m[14] +
	m[8]  * m[6]  * m[15] -
	m[8]  * m[7]  * m[14] -
	m[12] * m[6]  * m[11] +
	m[12] * m[7]  * m[10];
	
    inv[8] = m[4]  * m[9] * m[15] -
	m[4]  * m[11] * m[13] -
	m[8]  * m[5] * m[15] +
	m[8]  * m[7] * m[13] +
	m[12] * m[5] * m[11] -
	m[12] * m[7] * m[9];
	
    inv[12] = -m[4]  * m[9] * m[14] +
	m[4]  * m[10] * m[13] +
	m[8]  * m[5] * m[14] -
	m[8]  * m[6] * m[13] -
	m[12] * m[5] * m[10] +
	m[12] * m[6] * m[9];
	
    inv[1] = -m[1]  * m[10] * m[15] +
	m[1]  * m[11] * m[14] +
	m[9]  * m[2] * m[15] -
	m[9]  * m[3] * m[14] -
	m[13] * m[2] * m[11] +
	m[13] * m[3] * m[10];
	
    inv[5] = m[0]  * m[10] * m[15] -
	m[0]  * m[11] * m[14] -
	m[8]  * m[2] * m[15] +
	m[8]  * m[3] * m[14] +
	m[12] * m[2] * m[11] -
	m[12] * m[3] * m[10];
	
    inv[9] = -m[0]  * m[9] * m[15] +
	m[0]  * m[11] * m[13] +
	m[8]  * m[1] * m[15] -
	m[8]  * m[3] * m[13] -
	m[12] * m[1] * m[11] +
	m[12] * m[3] * m[9];
	
    inv[13] = m[0]  * m[9] * m[14] -
	m[0]  * m[10] * m[13] -
	m[8]  * m[1] * m[14] +
	m[8]  * m[2] * m[13] +
	m[12] * m[1] * m[10] -
	m[12] * m[2] * m[9];
	
    inv[2] = m[1]  * m[6] * m[15] -
	m[1]  * m[7] * m[14] -
	m[5]  * m[2] * m[15] +
	m[5]  * m[3] * m[14] +
	m[13] * m[2] * m[7] -
	m[13] * m[3] * m[6];
	
    inv[6] = -m[0]  * m[6] * m[15] +
	m[0]  * m[7] * m[14] +
	m[4]  * m[2] * m[15] -
	m[4]  * m[3] * m[14] -
	m[12] * m[2] * m[7] +
	m[12] * m[3] * m[6];
	
    inv[10] = m[0]  * m[5] * m[15] -
	m[0]  * m[7] * m[13] -
	m[4]  * m[1] * m[15] +
	m[4]  * m[3] * m[13] +
	m[12] * m[1] * m[7] -
	m[12] * m[3] * m[5];
	
    inv[14] = -m[0]  * m[5] * m[14] +
	m[0]  * m[6] * m[13] +
	m[4]  * m[1] * m[14] -
	m[4]  * m[2] * m[13] -
	m[12] * m[1] * m[6] +
	m[12] * m[2] * m[5];
	
    inv[3] = -m[1] * m[6] * m[11] +
	m[1] * m[7] * m[10] +
	m[5] * m[2] * m[11] -
	m[5] * m[3] * m[10] -
	m[9] * m[2] * m[7] +
	m[9] * m[3] * m[6];
	
    inv[7] = m[0] * m[6] * m[11] -
	m[0] * m[7] * m[10] -
	m[4] * m[2] * m[11] +
	m[4] * m[3] * m[10] +
	m[8] * m[2] * m[7] -
	m[8] * m[3] * m[6];
	
    inv[11] = -m[0] * m[5] * m[11] +
	m[0] * m[7] * m[9] +
	m[4] * m[1] * m[11] -
	m[4] * m[3] * m[9] -
	m[8] * m[1] * m[7] +
	m[8] * m[3] * m[5];
	
    inv[15] = m[0] * m[5] * m[10] -
	m[0] * m[6] * m[9] -
	m[4] * m[1] * m[10] +
	m[4] * m[2] * m[9] +
	m[8] * m[1] * m[6] -
	m[8] * m[2] * m[5];
	
    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
	
    if (det == 0)
        return;
	
    det = 1.0 / det;
	
    for (i = 0; i < 16; i++)
        invOut[i] = inv[i] * det;
}
void mtx_invert3x3(float *a, const float *b) {
	
}


void mtx_reduce4x4to3x3(float *A, const float *B) {
	// [ 0  4  8  - ]      [ 0  3  6 ]
	// [ 1  5  9  - ]  ->  [ 1  4  7 ]
	// [ 2  6  10 - ]      [ 2  5  8 ]
	// [ -  -  -  - ]
	A[0] = B[0];   A[3] = B[4];   A[6] = B[8];
	A[1] = B[1];   A[4] = B[5];   A[7] = B[9];
	A[2] = B[2];   A[5] = B[6];   A[8] = B[10];
}

