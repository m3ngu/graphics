/*
 *  Camera.cpp
 *  hw4_proj
 */

#include "Camera.h"

Camera::Camera (vec3 eye, vec3 lookAt, vec3 up) {
	
	Camera::eye = eye;
	
	// These 2 vectors will help us construct the new coordinate frame
	vec3 a, b;
	
	// a is the looking direction (from the eye to the original origin)
	a = eye - lookAt;
	a.normalize();
	
	// w is the z-axis of the new coordinate frome
	Camera::w = a;
	
	// We take the cross product of the up vector and the new z-axis to be our new y-axis
	b = up;
	cross(Camera::u, b, Camera::w);
	Camera::u.normalize();
	
	// Finally we take the cross product of the new z- and y-axes to get our 3rd and final axis
	cross(Camera::v, Camera::w, Camera::u);
	
	/*
	printf("Debug: u x=%f y=%f z=%f\n", Camera::u.x, Camera::u.y, Camera::u.z);
	printf("Debug: v x=%f y=%f z=%f\n", Camera::v.x, Camera::v.y, Camera::v.z);
	printf("Debug: w x=%f y=%f z=%f\n", Camera::w.x, Camera::w.y, Camera::w.z);
	*/
	
}

Camera::~Camera () {
}
		
void Camera::setupFov(float fovy, int* width, int* height) {
	
	float aspect_ratio = ((float) *width / (float) *height);
	
	//printf("Debug: width=%d, height=%d, aspect_ratio=%f\n", *width, *height, aspect_ratio);
	
	Camera::fovy = fovy * (nv_pi / 180.0f);
	
	Camera::fovx = aspect_ratio * Camera::fovy;
	
	//printf("Debug: fovy=%f, fovx=%f\n", Camera::fovy, fovx);
}