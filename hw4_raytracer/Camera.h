/*
 *  Camera.h
 */

#include <iostream>
#include "nv/nv_math.h"
#include "nv/nv_algebra.h"

class Camera {
	vec3 eye;
	vec3 lookAt;
	vec3 up;
	
	float fovx;
	float fovy;
	
	vec3 u;
	vec3 v;
	vec3 w;
	
	public:
		Camera (vec3 eye, vec3 lookAt, vec3 up);
		~Camera ();
		
		void setupFov(float fovy, int* width, int* height);
		
		vec3 * getU() { return &u; };
		vec3 * getV() { return &v; };
		vec3 * getW() { return &w; };
		
		vec3 * getEye() { return &eye; };
		
		float getFovX() { return fovx; };
		float getFovY() { return fovy; };
};