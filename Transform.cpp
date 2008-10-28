// Transform.cpp: implementation of the Transform class.
//
//////////////////////////////////////////////////////////////////////

#include "Transform.h"

bool mapOn = false;
vec3 storedEye, storedLookAt, storedUp;

void Transform::forward(float amount, vec3& eye, vec3& lookAt, float xMax, float yMax, float zMax) {
	vec3 lookingDirection = lookAt - eye;
	lookingDirection.normalize();
	eye += (amount * lookingDirection);
	
	// Don't let users move below the ground-plane
	if (eye.z < 2) {
		eye.z = 2;
	} else if (eye.z > zMax) {
		eye.z = zMax;
	} 
	
	if (eye.x < -xMax) {
		eye.x = -xMax;
	} else if (eye.x > xMax) {
		eye.x = xMax;
	} 
	
	if (eye.y < -yMax) {
		eye.y = -yMax;
	} else if (eye.y > yMax) {
		eye.y = yMax;
	} 
	
	lookAt = eye + lookingDirection;
}

void Transform::sideways(float amount, vec3& eye, vec3& lookAt, vec3& up, float xMax, float yMax) {

	vec3 lookingDirection = lookAt - eye;
	vec3 right;
	
	cross(right, lookingDirection, up);
	
	right.normalize();
	eye += (amount * right);
	
	if (eye.x < -xMax) {
		eye.x = -xMax;
	} else if (eye.x > xMax) {
		eye.x = xMax;
	} 
	
	if (eye.y < -yMax) {
		eye.y = -yMax;
	} else if (eye.y > yMax) {
		eye.y = yMax;
	} 
	
	lookAt = eye + lookingDirection;
}

//Takes as input the current eye position, and the current up vector.
//up is always normalized to a lenght of 1.
//eye has a length indicating the distance from the viewer to the origin
void Transform::left(float degrees, vec3& eye, vec3& lookAt, vec3& up) {
	float radians;
	mat3 R;
	vec3 lookingDirection = lookAt - eye;
	
	// First, we convert the rotation angle from degrees to radians
	// (1 deg = pi/180 rad)
	radians = (degrees/180.00) * nv_pi;
	
	// Next we construct a 3x3 rotation matrix using the "set_rot"
	// method of mat3, where the first parameter is the rotation angle
	// and second parameter is the rotation axis.  In this case, we're
	// rotating from right-to-left (i.e. around the up-axis)
	//R.set_rot(radians, up);
	vec3 z = vec3(0,0,1);
	R.set_rot(radians, z);
	
	// Finally, we apply the location of the eye vector by simply multiplying
	// it with the rotation matrix we've just created.
	lookingDirection = R * lookingDirection;
	lookAt = eye + lookingDirection;
	up = R * up;

}

void Transform::up(float degrees, vec3& eye, vec3& lookAt, vec3& up) {
	float radians;
	mat3 R;
	vec3 axis;
	vec3 lookingDirection = lookAt - eye;
	
	// Similar to the left rotation, we first convert the rotation angle to radians
	radians = (degrees/180.00) * nv_pi;
	
	// Next we need to determine the rotation axis.  For the upward rotation, the axis
	// is orthogonal to both the up-direction and the eye-direction (i.e. the y-axis).
	// We can get that vector easily by taking the cross-product of up and eye and normalizing
	cross(axis, lookingDirection, up);
	axis.normalize();
	
	// Once again, we create the rotation matrix using "mat3.set_rot"
	R.set_rot(radians, axis);
	
	// And we apply the rotation to both the eye- and the up-vectors
	lookingDirection = R * lookingDirection;
	lookAt = eye + lookingDirection;
	up = R * up;
}

void Transform::straighten(vec3& eye, vec3& lookAt, vec3& up) {
	lookAt.z = eye.z;
	up = vec3(0,0,1);
}

void Transform::toggleMapView(vec3& eye, vec3& lookAt, vec3& up) {
	
	if (mapOn) {
	
		eye = storedEye;
		lookAt = storedLookAt;
		up = storedUp;
	
		mapOn = false;
		
	} else {
		
		storedEye = eye;
		storedLookAt = lookAt;
		storedUp = up;
		
		eye = vec3(0,0,150);
		lookAt = vec3(0,0,0);
		up = vec3(0,1,0);
		
		mapOn = true;
	}
	
}

/*
void Transform::entireScene() {
	// How can I automatically calculate a view that displays my entire model?
	// (I know the bounding sphere and up vector.)

	// First, compute a bounding sphere for all objects in your scene. This should provide you
	// with two bits of information: the center of the sphere (let ( c.x, c.y, c.z ) be that
	// point) and its diameter (call it "diam").
	
	// Next, choose a value for the zNear clipping plane. General guidelines are to choose
	// something larger than, but close to 1.0. So, let's say you set

	zNear = 1.0; zFar = zNear + diam; 

	// Structure your matrix calls in this order (for an Orthographic projection):

	GLdouble left = c.x - diam;
	GLdouble right = c.x + diam;
	GLdouble bottom c.y - diam;
	GLdouble top = c.y + diam;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, zNear, zFar);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); 

	// This approach should center your objects in the middle of the window and stretch them
	// to fit (i.e., its assuming that you're using a window with aspect ratio = 1.0).
	// If your window isn't square, compute left, right, bottom, and top, as above, and put
	// in the following logic before the call to glOrtho():

	GLdouble aspect = (GLdouble) windowWidth / windowHeight;
	
	if ( aspect < 1.0 ) {
		// window taller than wide
		bottom /= aspect;
		top /= aspect;
	} else {
		left *= aspect;
		right *= aspect;
	} 

	// The above code should position the objects in your scene appropriately.
	// If you intend to manipulate (i.e. rotate, etc.), you need to add a viewing transform to it.

	// A typical viewing transform will go on the ModelView matrix and might look like this:

	GluLookAt (0., 0., 2.*diam, c.x, c.y, c.z, 0.0, 1.0, 0.0); 
}
*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



Transform::Transform()
{

}

Transform::~Transform()
{

}
