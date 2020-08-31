#include "Camera.h"
#include "glm_include.h"

#define VIEWMATRIX (*(glm::mat4*)mtx_view)
#define PROJMATRIX (*(glm::mat4*)mtx_proj)


Camera::Camera()
{
	pos = new float[3] { 0.0, 0.0, 0.0 };
	mtx_view = new glm::mat4;
	mtx_proj = new glm::mat4;
}

Camera::~Camera()
{
	delete [] pos;
	delete (glm::mat4*) mtx_view;
	delete (glm::mat4*) mtx_proj;
}


float* Camera::getViewMatrix() {
	return mptr(VIEWMATRIX);
}
float* Camera::getProjMatrix() {
	return mptr(PROJMATRIX);
}
void Camera::setViewMatrix(float *_V) {
	VIEWMATRIX = *(glm::mat4*)_V;   // This is a terrible idea
}


void Camera::setPosition(v3 p) {
	pos[0] = p.x;
	pos[1] = p.y;
	pos[2] = p.z;
}
void Camera::setOrientation(float angle, v3 v) { //float vx, float vy, float vz) {
	glm::mat4 invR = glm::rotate(glm::mat4(1.0),
								 -RAD(angle),
								 glm::vec3(v.x, v.y, v.z));
	
	// The view transform V is the inverse of the camera transitioning transform, C
	// Since C = R · T,
	//       V = inv(T) · inv(R)
	// In OGL the order is reversed, so we return inv(R) · inv(T)
	glm::mat4 invT = glm::translate(glm::mat4(1.0),
									glm::vec3(-pos[0], -pos[1], -pos[2]));
	VIEWMATRIX = glm::mat4(invR * invT);
}
void Camera::setLookFromTo(float aX, float aY, float aZ, float bX, float bY, float bZ) {
	auto A = glm::vec3(aX, aY, aZ);
	auto B = glm::vec3(bX, bY, bZ);
	VIEWMATRIX = glm::lookAt(A, B, glm::vec3(0, 1, 0));
}

void Camera::setPerspective(float fov, float w, float h, float zN, float zF) {
	PROJMATRIX = glm::mat4(glm::perspective(RAD(fov), w/h, zN, zF));
}

void Camera::setOrthographic(float l, float r, float b, float t, float zN, float zF) {
	PROJMATRIX = glm::mat4(glm::ortho(l, r, b, t, zN, zF));
}

void Camera::setPixel(float winW, float winH) {
	PROJMATRIX = glm::ortho(0.f, float(winW), float(winH), 0.f, 1.f, -1.f);
}

