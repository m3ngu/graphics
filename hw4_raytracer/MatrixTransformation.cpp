#include "MatrixTransformation.h"


//mat4 identityMatrix() {
//	mat4 identity = mat4(1, 0, 0, 0, 
//						0, 1, 0, 0, 
//						0, 0, 1, 0, 
//						0, 0, 0, 1);
//	return identity;
//}

//MatrixTransforms::getInverse(mat4 matrix)
//{
//	mat4 inverseMat;
//	invert(inverseMat,matrix);
//	return inverseMat;
//}

void MatrixTransforms::getRotate(vec3 rotatevec, float angle_deg, mat4 & rotation)
{
	float angle_rad = (180.0/nv_pi)*angle_deg;
	rotatevec.normalize();
	rotation = mat4_id;
	rotation.set_rot(angle_rad,rotatevec);
}

void MatrixTransforms::getTranslate(vec3 translatevec, mat4 & translation)
{
	translation = mat4_id;
	translation.set_translation(translatevec);
}

void MatrixTransforms::getScale(vec3 scalevec, mat4 & scaleMatrix)
{
	scaleMatrix = mat4(scalevec.x, 0, 0, 0,
							0, scalevec.y, 0, 0,
							0, 0, scalevec.z, 0,
							0, 0, 0, 1);
}
