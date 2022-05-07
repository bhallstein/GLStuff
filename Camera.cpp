#include "Camera.h"

glm::quat Camera::identity_quaternion(1,0,0,0);


void Camera::swivel_around_target(float angle) {
	glm::vec3 p = pos - pos_target;
	glm::quat q = glm::rotate(identity_quaternion, angle, {0,1,0});
	glm::vec3 p_rotated = q * p;
	pos = p_rotated + pos_target;
}

// Translation and rotation
// ---------------------------------

// TODO: lookAt funcs
// void Camera::lookAt(glm::vec3 from, glm::vec3 to) {
//   pos = from;
//   // To generate R, move both vectors so that 'from' is at the origin, then use glm::lookAt
//   // R = glm::lookAt({0., 0., 0.}, to - from, {0., 1., 0.});
// }
//
// void Camera::lookAt__preservingPos(glm::vec3 p) {
//   // R = glm::lookAt({0,0,0}, p - pos, {0,1,0});
// }
//
// void Camera::lookAt__preservingRot(glm::vec3 p) {
//   // Preserving the tilt and camera height (Y), set a new position
//   // float cam_new_z = x.z + p.y / tanf(tilt);
//   // setPosition( = { x.x, p.y, cam_new_z };
// }

// void Camera::setTilt(float t) {
//   rot = glm::rotate(identity_quaternion, -t, {1,0,0});
// }
// void Camera::setRotation(float r) {
//   rot = glm::rotate(identity_quaternion, r, {0,1,0});
// }
// void Camera::setTiltAndRotation(float t, float r) {
//   setTilt(t);
//   rot = glm::rotate(rot, r, {0,1,0});
// }
//
// void Camera::incRotation(float t) {
//   rot = glm::rotate(rot, t, {0,1,0});
// }


// Projection
// ---------------------------------

void Camera::setPerspective(float fov, float w, float h, float zN, float zF) {
	proj_matrix = glm::mat4(glm::perspective(RAD(fov), w/h, zN, zF));
}

void Camera::setOrthographic(float l, float r, float b, float t, float zN, float zF) {
	proj_matrix = glm::mat4(glm::ortho(l, r, b, t, zN, zF));
}

void Camera::setPixel(float winW, float winH) {
	proj_matrix = glm::ortho(0.f, float(winW), float(winH), 0.f, 1.f, -1.f);
}


glm::mat4 Camera::view_matrix() {
	// The view transform V is the inverse of the camera transitioning transform, C
	// Since C = R · T,
	//       V = inv(T) · inv(R)
	// In OGL the order is reversed, so we return inv(R) · inv(T)

	return glm::lookAt(pos, pos_target, {0,1,0});

	// glm::mat4 T = glm::translate(glm::mat4(1), pos);
	// glm::mat4 R = glm::mat4_cast(rot);
	//
	// return glm::inverse(T * R);
}



