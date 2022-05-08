#ifndef CAMERA_H
#define CAMERA_H

#include "CoordinateTypes.hpp"
#include "glm_include.hpp"

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

	void swivel_around_target(float angle) {
		glm::vec3 p = pos - pos_target;
		glm::quat q = glm::rotate(identity_quaternion(), angle, {0,1,0});
		glm::vec3 p_rotated = q * p;
		pos = p_rotated + pos_target;
	}

	// Projection
	void setPerspective(float fov, float w, float h, float zN, float zF) {
		proj_matrix = glm::mat4(glm::perspective(RAD(fov), w/h, zN, zF));
	}
	void setOrthographic(float left, float right, float btm, float top, float zN, float zF) {
		proj_matrix = glm::mat4(glm::ortho(left,
																			 right,
																			 btm,
																			 top,
																			 zN,
																			 zF));
	}
	void setPixel(float winw, float winh) {
		proj_matrix = glm::ortho(0.f, float(winw), float(winh), 0.f, 1.f, -1.f);
	}

	glm::mat4 view_matrix() {
		return glm::lookAt(pos, pos_target, {0,1,0});
	}
	glm::mat4 proj_matrix;
	static glm::quat identity_quaternion() {
		return glm::quat(1,0,0,0);
	}
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
