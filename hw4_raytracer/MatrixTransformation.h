#include "nv/nv_math.h"

class MatrixTransforms {
public:
	MatrixTransforms(){};
	void getTranslate(vec3 translatevec, mat4 & translation);
	void getRotate(vec3 rotatevec, float angle_deg, mat4 & rotation);
	void getScale(vec3 scalevec, mat4 & scaleMatrix);
};