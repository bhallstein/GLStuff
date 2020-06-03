

#include "Camera.h"
#include "glm_include.h"


Camera::Camera()
{
	pos = new float[3] { 0.0, 0.0, 0.0 };
	mtx_view = new glm::mat4;
	mtx_proj = new glm::mat4;
	
	mtx_invrot = new glm::mat4;
}

Camera::~Camera()
{
	delete [] pos;
	delete (glm::mat4*) mtx_view;
	delete (glm::mat4*) mtx_proj;
	delete (glm::mat4*) mtx_invrot;
}


float* Camera::getViewMatrix() {
	// The view transform V is the inverse of the camera transitioning transform, C
	// Since C = R · T,
	//       V = inv(T) · inv(R)
	// In OGL the order is reversed, so we return inv(R) · inv(T)

	glm::mat4 &V = *(glm::mat4*)mtx_view;
	
	if (using_orientation) {
		glm::mat4 invT = glm::translate(glm::mat4(1.0),
										glm::vec3(-pos[0], -pos[1], -pos[2]));
		V = glm::mat4(*(glm::mat4*)mtx_invrot * invT);
	}
	
	return mptr(V);
}
float *Camera::getProjMatrix() {
	glm::mat4 &P = *(glm::mat4*)mtx_proj;
	return mptr(P);
}


void Camera::setPosition(float x, float y, float z) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}

void Camera::setOrientation(float angle, float vx, float vy, float vz) {
	glm::mat4 &R = * (glm::mat4*) mtx_invrot;
	R = glm::rotate(glm::mat4(1.0),
					-RAD(angle),
					glm::vec3(vx, vy, vz));
	
	using_orientation = true;
}
void Camera::setLookFromTo(float aX, float aY, float aZ, float bX, float bY, float bZ) {
	glm::vec3 A = glm::vec3(aX, aY, aZ);
	glm::vec3 B = glm::vec3(bX, bY, bZ);
	
	glm::mat4 &V = * (glm::mat4*) mtx_view;
	V = glm::lookAt(A, B, glm::vec3(0, 1, 0));
	
	using_orientation = false;
}

void Camera::setPerspective(float fov, float w, float h, float zN, float zF) {
	glm::mat4 &P = *(glm::mat4*)mtx_proj;
	P = glm::mat4(glm::perspective(RAD(fov), w/h, zN, zF));
}

void Camera::setOrthographic(float l, float r, float b, float t, float zN, float zF) {
	glm::mat4 &P = *(glm::mat4*)mtx_proj;
	P = glm::mat4(glm::ortho(l, r, b, t, zN, zF));
}

void Camera::setPixel(float winW, float winH) {
	glm::mat4 &P = *(glm::mat4*)mtx_proj;
	P = glm::ortho(0.f, float(winW), float(winH), 0.f, 1.f, -1.f);
}

