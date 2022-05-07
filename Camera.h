#ifndef __CAMERA_H
#define __CAMERA_H

#include "CoordinateTypes.h"
#include "glm_include.h"

struct Camera {
	glm::vec3 pos;
	glm::vec3 pos_target;

	// void lookAt(glm::vec3 from, glm::vec3 to);
	// void lookAt__preservingPos(glm::vec3 p);
	// void lookAt__preservingRot(glm::vec3 p);
	// void setTilt(float);
	// void setRotation(float);
	// void setTiltAndRotation(float tilt, float rotation);

	void nudge(glm::vec3 delta) {
		pos += delta;
		pos_target += delta;
	}

	void swivel_around_target(float angle);

	// Projection
	void setPerspective(float fov, float w, float h, float zN, float zF);
	void setOrthographic(float left, float right, float bottom, float top, float zN, float zF);
	void setPixel(float winW, float winH);

	// float yaw;      // Rotation about the canonical y axis
	// float pitch;    // Rotation about the rotated x axis
	// float roll;     // Rotation about the rotated & tilted z axis. Usually zero
	//
	// glm::quat rotation_quaternion();

	glm::mat4 view_matrix();  // Recalculate dynamically
	glm::mat4 proj_matrix;
	static glm::quat identity_quaternion;
};


// Top-down camera
// --------------------------------------

struct TopDownCamera {
	constexpr static float top_down_cam__fov = 67.;
	constexpr static float top_down_cam__znear = 0.1;
	constexpr static float top_down_cam__zfar = 1000.;

	static Camera mk(float win_w, float win_h) {
		Camera cam;

		cam.pos = {0,12,14};
		cam.pos_target = {0,0,0};
		// cam.setTilt(30./180. * M_PI);

		update_projection(cam, win_w, win_h);

		return cam;
	}

	static void update_projection(Camera &cam, float win_w, float win_h) {
		cam.setPerspective(top_down_cam__fov, win_w, win_h, top_down_cam__znear, top_down_cam__zfar);
	}

	static void world_swivel(Camera &cam, glm::ivec2 win_from, glm::ivec2 win_to) {
		// Given 2 window coords, move and rotate the camera to keep the selected
		// world floor point under the mouse. The camera stays upright.
	}
};

#endif


