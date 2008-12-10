#include<stdio.h>
#include "Material.h"
#include "nv/nv_math.h"

class Intersect {

public:
	double t;
	vec3 Point;
	vec3 rayOrigin;
	vec3 rayDirection;
	vec3 normal;
	Material* mat;
//	obj currobject;
	Intersect();
	Intersect(vec3 origin, vec3 direction, double tmax);
	void setPoint();
	void setNormal();
	virtual ~Intersect() {};
};
