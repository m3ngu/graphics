#include<stdio.h>
#include "nv/nv_math.h"
#include "Intersection.h"

static float absolute(float num);
static float const err = 0.0000001;

enum shape_type {
	SPHERE,
	QUAD,
	TRIANGLE
};

class obj
{
public:
	obj(){};
	virtual ~obj(){};
	mat4 TransformMatrix;
	virtual bool intersect(vec3&, vec3&, Intersect *intObj) = 0;
	virtual void getNormal(vec3& normal, vec3 intersectPoint) = 0;
	Material mat;
	shape_type type;
	bool debug;
};

class quad : public obj
{
private:
	vec3 V00, V10, V11, V01; //vertices of the quad
	vec3 E01, E02, E03; //vectors defining the quad edges
	vec3 N; //normal to the plane containing the quad
	float a11, b11; //compute barycentric co-ods of V11
	float err; //find the appropriate value by trial and error

public:
	quad(){};
	// Creates a new quad using the 4 vertices
	quad(vec3,vec3,vec3,vec3);

	//uses Ray-Quadrilateral Intersection Test given at
	//http://www.cs.kuleuven.be/~graphics/CGRG.PUBLICATIONS/LagaeDutre2005AnEfficientRayQuadrilateralIntersectionTest/paper.pdf
	//finds if the ray intersects the quadrilatral given, returning true or false

	//overide the base class method
	virtual bool intersect(vec3&, vec3&, Intersect *intObj);
	virtual void getNormal(vec3& normal, vec3 intersectPoint);
	virtual ~quad() {};

};


class triangle : public obj
{
private:
	vec3 A, B, C;
	float err;
	static const int TEST_CULL = 1;
public:
	triangle(){};
	triangle(vec3, vec3, vec3);
	virtual bool intersect(vec3&, vec3&, Intersect *intObj);
	virtual void getNormal(vec3& normal, vec3 intersectPoint);
	virtual ~triangle() {};
};


class sphere : public obj
{
private:
	vec3 center;
	float radius;
	float radius2;
public:
	sphere(){};
	// Creates a new quad using the 4 vertices
	sphere(float,vec3);
	//overide the base class method
	virtual bool intersect(vec3&, vec3&, Intersect *intObj);
	virtual void getNormal(vec3& normal, vec3 intersectPoint);
	virtual ~sphere() {};
};
