

#include "Camera.h"
#include "glm_include.h"


Camera::Camera()
{
	pos = new float[3] { 0.0, 0.0, 0.0 };
	rotationAngle = 0.0;
	rotationVec = new float[3] { 1.0, 0.0, 0.0 };
	mtx_view = NULL;
	mtx_proj = NULL;
}

Camera::~Camera()
{
	delete [] pos;
	delete [] rotationVec;
	if (mtx_view) delete (glm::mat4*) mtx_view;
	if (mtx_proj) delete (glm::mat4*) mtx_proj;
}


const float* Camera::getViewMatrix() {
	// The view transform V is the inverse of the camera transitioning transform, C
	// Since C = R · T,
	//       V = inv(T) · inv(R)
	// In OGL the order is reversed, so we return inv(R) · inv(T)
	
	glm::mat4 invR = glm::rotate(glm::mat4(1.0),
								 -rotationAngle,
								 glm::vec3(rotationVec[0], rotationVec[1], rotationVec[2]));
	glm::mat4 invT = glm::translate(glm::mat4(1.0),
									glm::vec3(pos[0], pos[1], pos[2]));
	if (mtx_view) delete (glm::mat4*) mtx_view;
	mtx_view = new glm::mat4(invR * invT);
	
	return mptr(*(glm::mat4*)mtx_view);
}
const float *Camera::getProjMatrix() {
	if (mtx_proj == NULL) return NULL;
	return mptr(*(glm::mat4*)mtx_proj);
}


void Camera::setPosition(float x, float y, float z) {
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
}

void Camera::setOrientation(float angle, float vx, float vy, float vz) {
	rotationAngle = angle;
	rotationVec[0] = vx;
	rotationVec[1] = vy;
	rotationVec[2] = vz;
}

void Camera::setPerspective(float fov, float w, float h, float zN, float zF) {
	if (mtx_proj) delete (glm::mat4*) mtx_proj;
	mtx_proj = new glm::mat4(glm::perspective(fov, w/h, zN, zF));
}

void Camera::setOrthographic(float l, float r, float b, float t, float zN, float zF) {
	if (mtx_proj) delete (glm::mat4*) mtx_proj;
	mtx_proj = new glm::mat4(glm::ortho(l, r, b, t, zN, zF));
}
