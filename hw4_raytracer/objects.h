#ifndef __objects__
#define __objects__

#include <stdio.h>
#include "nv/nv_math.h"
#include "Intersection.h"
#include "Material.h"

static float absolute(float num);
static float const err = 0.0000001;

enum shape_type {
	SPHERE,
	QUAD,
	TRIANGLE
};

enum normal_type {
	SURFACE,
	VERTEX
};

// =============================================================================
//     OBJ
// =============================================================================

class obj
{
public:
	obj(){};
	virtual ~obj(){};
	virtual bool intersect(vec3&, vec3&, Intersect *intObj) = 0;
	virtual void getNormal(vec3& normal, vec3 intersectPoint) = 0;
	mat4 TransformMatrix;
	Material mat;
	shape_type type;
	bool debug;
};

// =============================================================================
//     QUAD
// =============================================================================

class quad : public obj
{
private:
	vec3 V00, V10, V11, V01; //vertices of the quad
	vec3 E01, E02, E03; //vectors defining the quad edges
	float a11, b11; //compute barycentric co-ods of V11
	float err; //find the appropriate value by trial and error

public:
	
	vec3 surf_normal;
	
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

// =============================================================================
//     TRIANGLE
// =============================================================================

class triangle : public obj
{
private:
	vec3 A, B, C;
	float u, v;
	float err;
	static const int TEST_CULL = 1;
public:

	normal_type ntype;
	vec3 surf_normal;
	vec3 vert_normal[3];

	triangle(){};
	triangle(vec3, vec3, vec3);
	triangle(vec3, vec3, vec3, vec3, vec3, vec3);
	virtual bool intersect(vec3&, vec3&, Intersect *intObj);
	virtual void getNormal(vec3& normal, vec3 intersectPoint);
	virtual ~triangle() {};
};

// =============================================================================
//     SPHERE
// =============================================================================

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

#endif // __objects__