
#ifndef __CAMERA_H
#define __CAMERA_H


class Camera {
public:
	Camera();
	~Camera();
	
	float* getViewMatrix();
	float* getProjMatrix();
	
	void setPosition(float x, float y, float z);
	void setOrientation(float angle, float vx, float vy, float vz);
	void setLookFromTo(float aX, float aY, float aZ, float bX, float bY, float bZ);
	
	float *pos;
		// The default position is (0,0,0)
	
	void setPerspective(float fov, float w, float h, float zN, float zF);
	void setOrthographic(float left, float right, float bottom, float top, float zN, float zF);
	void setPixel(float winW, float winH);
	
private:
	bool using_orientation;
	void *mtx_view;
	void *mtx_proj;
	void *mtx_invrot;
};

#endif
