
#ifndef __CAMERA_H
#define __CAMERA_H

class Camera {
public:
	Camera();
	virtual ~Camera();
	
	const float* getViewMatrix();
	const float* getProjMatrix();
	
	void setPosition(float x, float y, float z);
	void setOrientation(float angle, float vx, float vy, float vz);
	
	float *pos;
		// The default position is (0,0,0)

	float rotationAngle;
	float *rotationVec;
		// The default orientation looks down the -z axis.
		// This corresponds to an identity transf. in OGL.
		// You can rotate from here x° about v.
		// v is (1, 0, 0) by default
	
	void setPerspective(float fov, float w, float h, float zN, float zF);
	void setOrthographic(float left, float right, float bottom, float top, float zN, float zF);
	
private:
	void *mtx_view;
	void *mtx_proj;
	
};

#endif
