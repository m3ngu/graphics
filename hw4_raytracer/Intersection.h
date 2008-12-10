#ifndef __intersect__
#define __intersect__

#include <stdio.h>
#include "nv/nv_math.h"
#include "Material.h"
#include "MatrixTransformation.h"
//#include "objects.h"

class Intersect {

public:
	double t;
	vec3 Point;
	vec3 rayOrigin;
	vec3 rayDirection;
	vec3 normal;
	Material* mat;
	//obj* currobject;
	Intersect();
	Intersect(vec3 origin, vec3 direction, double tmax);
	void setPoint();
	void Intersect::setPoint(const vec3& org, const vec3& dir);
	void setNormal();
	void applyTransform(const mat4& m);
	void applyTransformToNormal(const mat4& m);
	virtual ~Intersect() {};
	bool debug;
};

#endif // __intersect__