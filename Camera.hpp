#ifndef Camera_h
#define Camera_h

#include "vectors.hpp"

struct Camera {
	v3 pos;
	v3 pos_target;

	// void lookAt(v3 from, v3 to);
	// void lookAt__preservingPos(v3 p);
	// void lookAt__preservingRot(v3 p);
	// void setTilt(float);
	// void setRotation(float);
	// void setTiltAndRotation(float tilt, float rotation);

	void nudge(v3 delta) {
		pos += delta;
		pos_target += delta;
	}

	void swivel_around_target(float angle) {
		v3 p = pos - pos_target;
		quat q = glm::rotate(identity_quaternion(), angle, {0,1,0});
		v3 p_rotated = q * p;
		pos = p_rotated + pos_target;
	}

	// Projection
	void setPerspective(float fov, float w, float h, float zN, float zF) {
		proj_matrix = m4(glm::perspective(RAD(fov), w/h, zN, zF));
	}
  void setOrthographic(float left, float right, float btm, float top, float zN, float zF) {
    proj_matrix = m4(glm::ortho(left,
                                right,
                                btm,
                                top,
                                zN,
                                zF));
  }
	void setPixel(float winw, float winh) {
		proj_matrix = glm::ortho(0.f, float(winw), float(winh), 0.f, 1.f, -1.f);
	}

	m4 view_matrix() {
		return glm::lookAt(pos, pos_target, {0,1,0});
	}
	m4 proj_matrix;
	static quat identity_quaternion() {
		return quat(1,0,0,0);
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

	static void world_swivel(Camera &cam, i2 win_from, i2 win_to) {
		// Given 2 window coords, move and rotate the camera to keep the selected
		// world floor point under the mouse. The camera stays upright.
	}
};

#endif 
