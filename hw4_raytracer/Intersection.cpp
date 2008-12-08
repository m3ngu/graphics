#include "Intersection.h"

Intersect::Intersect() {
	rayOrigin = vec3(0.0f, 0.0f, 0.0f);
	rayDirection =  vec3(0.0f, 0.0f, 0.0f);
	t = 0.0f;
}

Intersect::Intersect(vec3 origin, vec3 direction, double tmax) {
	rayOrigin = origin;
	rayDirection = direction;
	t = tmax;
}
void Intersect::setPoint() {
	Point = rayOrigin + t*rayDirection;
}