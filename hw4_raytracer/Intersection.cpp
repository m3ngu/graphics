#include "Intersection.h"

Intersect::Intersect() {
	rayOrigin = vec3(0.0f, 0.0f, 0.0f);
	rayDirection =  vec3(0.0f, 0.0f, 0.0f);
	normal = vec3(0.0f, 0.0f, 0.0f);
	t = 0.0f;
	debug = false;
}

Intersect::Intersect(vec3 origin, vec3 direction, double tmax) {
	rayOrigin = origin;
	rayDirection = direction;
	t = tmax;
	debug = false;
}
void Intersect::setPoint() {
	Point = rayOrigin + t*rayDirection;
}

void Intersect::setPoint(const vec3& org, const vec3& dir) {
	Point = org + t * dir;
}

void Intersect::applyTransform(const mat4& m) {
	vec3 pt;
	MatrixTransforms mobj;
	mobj.getVector(Point,m,pt,0);
	Point = pt;
}

void Intersect::applyTransformToNormal(const mat4& m) {
	vec3 n;
	MatrixTransforms mobj;
	mobj.getVector(normal,m,n,1);
	n.normalize();
	normal = n;
}
