#include<stdio.h>
#include "nv/nv_math.h"
#include "Material.h"

class Intersect {
private:
	vec3 Point;
public:
	double t;
	vec3 rayOrigin;
	vec3 rayDirection;
	Material* mat;
	Intersect();
	Intersect(vec3 origin, vec3 direction, double tmax);
	void setPoint();
	virtual ~Intersect() {};
};
