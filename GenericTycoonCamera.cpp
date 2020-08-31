//
//  GenericTycoonCamera.cpp
//  ComponentTest
//
//  Created by Ben on 06/07/2016.
//  Copyright © 2016 Ben. All rights reserved.
//

#include "GenericTycoonCamera.h"
#include "glm_include.h"
#include "Vectors.h"


GenericTycoonCamera::GenericTycoonCamera() : p{0, 0, 0}, tilt(45.0)
{
	p = { 0, 12, 14 };
	tilt = 35.0 / 180. * M_PI;
	updateCamera();
}


void GenericTycoonCamera::updatePerspective(float winW, float winH) {
	cam.setPerspective(fov, winW, winH, zNear, zFar);
}


void GenericTycoonCamera::updateCamera() {
	// Regen view matrix from camera tilt & position
	auto T = glm::translate(glm::mat4(1.0), glm::vec3(p.x, p.y, p.z));
	auto R = glm::rotate(glm::mat4(1.0), -tilt, glm::vec3(1, 0, 0));
	auto M = glm::inverse(T * R);   // Tilt, then translate

	cam.setViewMatrix(mptr(M));
}


void GenericTycoonCamera::lookAt(v3 x) {
	// Preserving the tilt and camera height (Y), set a new position
	float cam_new_z = x.z + p.y / tanf(tilt);
	p = { x.x, p.y, cam_new_z };
	
	updateCamera();
}

