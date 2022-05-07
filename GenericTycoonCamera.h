//
//  GenericTycoonCamera.h
//  ComponentTest
//
//  Created by Ben on 06/07/2016.
//  Copyright © 2016 Ben. All rights reserved.
//

#ifndef _GenericTycoonCamera_h
#define _GenericTycoonCamera_h

#include "Camera.h"


class GenericTycoonCamera {
public:
	GenericTycoonCamera();
	
	void updatePerspective(float winW, float winH);
	void updateCamera();
	
	void setPos(v3 _p) { p = _p; }
	void setTilt(float _t) { tilt = _t; }
	
	void lookAt(v3 x);  // Preserve tilt, move position
	
	void nudgeX(float x) { p.x += x; updateCamera(); }
	void nudgeZ(float x) { p.z += x; updateCamera(); }
	void nudgeY(float x) { p.y += x; updateCamera(); }
	
	void tiltUD(float a) { tilt += a; updateCamera(); }
//	void rotateUD(float x) { cam.rotateUD(x); }
	
	Camera* getCam() { return &cam; }
	
private:
	Camera cam;
	
	// Position & orientation
	v3 p;
	float tilt;       // Tilt of the camera around the X axis down toward the -ve y axis
	
	// Perspective
	float fov = 67.0; // winH / 12; // 67;
	float zNear = 0.1;
	float zFar = 1000.;
};


#endif
