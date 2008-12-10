#ifndef __mattrans__
#define __mattrans__

#include "nv/nv_math.h"

class MatrixTransforms {
public:
	MatrixTransforms(){};
	void getTranslate(vec3 translatevec, mat4 & translation);
	void getRotate(vec3 rotatevec, float angle_deg, mat4 & rotation);
	void getScale(vec3 scalevec, mat4 & scaleMatrix);
	void getVector(vec3 givenVector, mat4 transformMatrix, vec3 & outputVector, int dirflag);
};

inline void MatrixTransforms::getVector(vec3 givenVector, mat4 transformMatrix, vec3 & outputVector, int dirflag) {
	vec4 homoVector;
	
	if(dirflag == 0)
		homoVector = vec4(givenVector.x,givenVector.y,givenVector.z,1);
	else
		homoVector = vec4(givenVector.x,givenVector.y,givenVector.z,0);

	vec4 newVector = transformMatrix * homoVector;
	outputVector = vec3(newVector.x,newVector.y,newVector.z);
}

inline void MatrixTransforms::getRotate(vec3 rotatevec, float angle_deg, mat4 & rotation)
{
	float angle_rad = (180.0/nv_pi)*angle_deg;
	rotatevec.normalize();
	rotation = mat4_id;
	rotation.set_rot(angle_rad,rotatevec);
}

inline void MatrixTransforms::getTranslate(vec3 translatevec, mat4 & translation)
{
	translation = mat4_id;
	translation.set_translation(translatevec);
}

inline void MatrixTransforms::getScale(vec3 scalevec, mat4 & scaleMatrix)
{
	scaleMatrix = mat4(scalevec.x, 0, 0, 0,
							0, scalevec.y, 0, 0,
							0, 0, scalevec.z, 0,
							0, 0, 0, 1);
}

#endif // __mattrans__