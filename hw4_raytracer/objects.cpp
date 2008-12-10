#include "objects.h"

// =============================================================================
//     SPHERE
// =============================================================================

sphere:: sphere(float r, vec3 c)
{
	debug = false;
	type = SPHERE;
	radius = r;
	center = c;
	radius2 = r*r;
}

void sphere::getNormal(vec3& normal,vec3 intersectPoint)
{
	normal = (intersectPoint - center); // / (float)radius;
	normal.normalize();
}
//Refer example 1
//TODO:: Figure out what is being done in the last 3 lines and the texture part

bool sphere::intersect(vec3& org, vec3& dir, Intersect *intObj)
{
	
	vec3 origin, direction;
	
	MatrixTransforms mobj;
	
	mobj.getVector(org, TransformMatrixInv, origin, 0);
	mobj.getVector(dir, TransformMatrixInv, direction, 1);

	//direction.normalize();
	
	// Vector from origin of Ray to center of sphere
	vec3 dist = origin - center;
	
	if (debug) printf("Debug: dist [%f, %f, %f]\n", dist.x, dist.y, dist.z);
	
	double a = dot(direction, direction);
	double b = 2*(dist.x*direction.x + dist.y*direction.y + dist.z*direction.z);
	double c = dot(dist,dist) - radius2;
	double tempt;
		//check if the discriminant is negative, if it is then return false, otherwise compute 
		//intersectionPosition and the rest
		double discriminant = (b*b) - (4*a*c);

		if(discriminant < 0.0)
			return false;
		else //find the intersection point
		{	
			//printf("Comes here and some color shud've been added\n");
			double t1 = (-b + sqrt(discriminant))/(2*a);
			double t2 = (-b - sqrt(discriminant))/(2*a);

			if( t1 < t2)
				tempt = t1;
			else
				tempt = t2;
				
			if (debug) printf("Debug: t1=%f, t2=%f, tempt=%f\n", t1, t2, tempt);

			//if (debug) printf("Debug: Sphere intersection! tempt=%f, t=%f\n",tempt, intObj->t);

			if( tempt < intObj->t && tempt > 0.0001f)
			{
				intObj->t = tempt;
				intObj->mat = &mat;
				intObj->setPoint(origin, direction);
				//intObj->finalObject = this;
				getNormal(intObj->normal, intObj->Point);
				
				if (debug) printf("Debug: Original Org [%f, %f, %f]\n", org.x, org.y, org.z);
				if (debug) printf("Debug: Original Dir [%f, %f, %f]\n", dir.x, dir.y, dir.z);
				if (debug) printf("Debug: Transform Org [%f, %f, %f]\n", origin.x, origin.y, origin.z);
				if (debug) printf("Debug: Transform Dir [%f, %f, %f]\n", direction.x, direction.y, direction.z);
				
				if (debug) printf("Debug: Point  [%f, %f, %f]\n", (intObj->Point).x, (intObj->Point).y, (intObj->Point).z);
				if (debug) printf("Debug: Normal [%f, %f, %f]\n", (intObj->normal).x, (intObj->normal).y, (intObj->normal).z);
				
				intObj->applyTransform(TransformMatrix);
				intObj->applyTransformToNormal(TransformMatrixInvTranspose);
				
				if (debug) printf("Debug: Point  [%f, %f, %f]\n", (intObj->Point).x, (intObj->Point).y, (intObj->Point).z);
				if (debug) printf("Debug: Normal [%f, %f, %f]\n", (intObj->normal).x, (intObj->normal).y, (intObj->normal).z);
				
				
				if (debug) {
					printf("Transform:\n");
					printf("%f, %f, %f, %f\n", TransformMatrix[0][0], TransformMatrix[0][1], TransformMatrix[0][2], TransformMatrix[0][3]);
					printf("%f, %f, %f, %f\n", TransformMatrix[1][0], TransformMatrix[1][1], TransformMatrix[1][2], TransformMatrix[1][3]);
					printf("%f, %f, %f, %f\n", TransformMatrix[2][0], TransformMatrix[2][1], TransformMatrix[2][2], TransformMatrix[2][3]);
					printf("%f, %f, %f, %f\n", TransformMatrix[3][0], TransformMatrix[3][1], TransformMatrix[3][2], TransformMatrix[3][3]);
				}
				
				if (debug) {
					printf("TransformMatrixInv:\n");
					printf("%f, %f, %f, %f\n", TransformMatrixInv[0][0], TransformMatrixInv[0][1], TransformMatrixInv[0][2], TransformMatrixInv[0][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInv[1][0], TransformMatrixInv[1][1], TransformMatrixInv[1][2], TransformMatrixInv[1][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInv[2][0], TransformMatrixInv[2][1], TransformMatrixInv[2][2], TransformMatrixInv[2][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInv[3][0], TransformMatrixInv[3][1], TransformMatrixInv[3][2], TransformMatrixInv[3][3]);
				}
				
				if (debug) {
					printf("TransformMatrixInvTranspose:\n");
					printf("%f, %f, %f, %f\n", TransformMatrixInvTranspose[0][0], TransformMatrixInvTranspose[0][1], TransformMatrixInvTranspose[0][2], TransformMatrixInvTranspose[0][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInvTranspose[1][0], TransformMatrixInvTranspose[1][1], TransformMatrixInvTranspose[1][2], TransformMatrixInvTranspose[1][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInvTranspose[2][0], TransformMatrixInvTranspose[2][1], TransformMatrixInvTranspose[2][2], TransformMatrixInvTranspose[2][3]);
					printf("%f, %f, %f, %f\n", TransformMatrixInvTranspose[3][0], TransformMatrixInvTranspose[3][1], TransformMatrixInvTranspose[3][2], TransformMatrixInvTranspose[3][3]);
				}
				
				
			}
			return true;
		}

}

// =============================================================================
//     QUAD
// =============================================================================

quad:: quad(vec3 A, vec3 B, vec3 C, vec3 D)  
{
	debug = false;
	type = QUAD;
	
	V00 = A ;
	V10 = B ;
	V11 = C ;
	V01 = D ;
	
	// Precalculating vectors for optimizations
	E01 = V10 - V00;
	E03 = V01 - V00;
	E02 = V11 - V00;

	cross(surf_normal, E01, E03);
	//surf_normal.normalize(); //not sure right now!
	
	//To find a11 and b11 for optimization
	if( (absolute(surf_normal.x) >= absolute(surf_normal.y))
	 && (absolute(surf_normal.x) >= absolute(surf_normal.z)) )
	{
		a11 = (E02.y*E03.z - E02.z*E03.y)/surf_normal.x;
		b11 = (E01.y*E02.z - E01.z*E02.y)/surf_normal.x;
	}
	else if( (absolute(surf_normal.y) >= absolute(surf_normal.x))
	      && (absolute(surf_normal.y) >= absolute(surf_normal.z)) )
	{
		a11 = (E02.z*E03.x - E02.x*E03.z)/surf_normal.y;
		b11 = (E01.z*E02.x - E01.x*E02.z)/surf_normal.y;
	}
	else
	{
		a11 = (E02.x*E03.y - E02.y*E03.x)/surf_normal.z;
		b11 = (E01.x*E02.y - E01.y*E02.x)/surf_normal.z;
	}
}

void quad::getNormal(vec3& normal, vec3 intersectPoint)
{
	normal = surf_normal;
}

bool quad::intersect(vec3& org, vec3& dir, Intersect *intObj)
{

	vec3 origin, direction;

	MatrixTransforms mobj;
	
	mobj.getVector(org, TransformMatrixInv, origin, 0);
	mobj.getVector(dir, TransformMatrixInv, direction, 1);

	vec3 P;
	float u, v;
	cross(P,direction,E03); //getting the point of intersection with the plane.

	float det = dot(E01,P);
	
	//Ray doesn't intersect the plane
	
	if(absolute(det) < err) {
		printf("the det=%f, absolute(det)=%f, err=%f\n",det,absolute(det),err);
		return false;
	}
	
	//Reject rays using barycentric coordinates of the intersection point wrt T
	vec3 T = origin - V00;
	float a = dot(T,P)/det;
	//The point lies outside the left of first triangle
	if(a > 1 || a < 0) {
		return false;
	}
	vec3 Q;
	cross(Q,T,E01);
	float b = dot(direction,Q)/det;	
	//The point lies outside the bottom of first triangle
	if(b > 1 || b < 0) {
		return false;
	}

	//Reject rays using barycentric coordinates of the intersection point wrt Tprime
	if((a+b) > 1) {
		vec3 E23 = V01 - V11;
		vec3 E21 = V10 - V11;
		vec3 Pprime;
		cross(Pprime,direction,E21);
		float detprime = dot(E23,Pprime);
		if(absolute(detprime) < err) {
			return false;
		}
		vec3 Tprime = origin - V11;
		float aprime = dot(Tprime,Pprime)/detprime;
		if (aprime < 0) {
			return false;
		}
		vec3 Qprime;
		cross(Qprime,Tprime,E23);
		float bprime = dot(direction,Qprime)/detprime;
		if (bprime < 0) {
			return false;
		}
	}

	//Compute the ray parameter of the intersection point
	float t1 = dot(E03,Q)/det;
	
	//if (debug) printf("Debug: Quad intersection! t1=%f, t=%f\n",t1, intObj->t);
	
	if(t1 > 0 && t1 < intObj->t) {
		intObj->t = t1;
		intObj->mat = &mat;
		intObj->setPoint(origin, direction);
		//intObj->finalObject = this;
		getNormal(intObj->normal, intObj->Point);
		intObj->applyTransform(TransformMatrix);
		intObj->applyTransformToNormal(TransformMatrixInvTranspose);
	} else {
		return false;
	}

	//Compute the bilinear coordinates of the intersection point
	if(absolute(a11-1) < err) {
		u = a;
		if(absolute(b11-1) < err) {
			v = b;
		}
		else {
			v = b/(u*(b11-1) + 1);
		}
	}
	else if(absolute(b11 -1) < err) {
		v = b;
		u = a/(v*(a11-1) + 1);
	}
	else {
		float A = -(b11 - 1);
		float B = a*(b11 - 1) - b*(a11 - 1) -1;
		float C = a;
		float delta = (b*b) - (4*A*C);
		float res;
		if(B > 0) {
			res = -1/2*(B + (float)sqrt(delta));
		}
		else {
			res = -1/2*(B - (float)sqrt(delta));
		}
		u = res/A;
		if((u < 0) || (u > 1)) {
			u = C/res;
		}
		v = b/(u*(b11 - 1) + 1);
	}
	
	return true;
	
}

// =============================================================================
//     TRIANGLE
// =============================================================================

triangle::triangle(vec3 P, vec3 Q, vec3 R)
{
	debug = false;
	type = TRIANGLE;
	ntype = SURFACE;
	
	A = P;
	B = Q;
	C = R;
	
	u = 0.0f;
	v = 0.0f;
	
	vec3 BA, CA;

    BA = B - A;
	CA = C - A;
    cross(surf_normal, BA, CA);
	surf_normal.normalize();
	
}

triangle::triangle(vec3 P, vec3 Q, vec3 R, vec3 NP, vec3 NQ, vec3 NR)
{
	triangle(P, Q, R);
	
	vert_normal[0] = NP;
	vert_normal[1] = NQ;
	vert_normal[2] = NR;
}


void triangle::getNormal(vec3& normal, vec3 intersectPoint)
{
	if (ntype == SURFACE) {
	
		normal = surf_normal;
		
	} else {
		
		vec3 interpolatedNormal = (1.0f-u-v) * vert_normal[0]
										 + u * vert_normal[1]
										 + v * vert_normal[2];
		interpolatedNormal.normalize();
		
		normal = interpolatedNormal;
	}
}

bool triangle::intersect(vec3 &org, vec3 &dir, Intersect *intObj)
{

	vec3 origin, direction;

	MatrixTransforms mobj;
	
	mobj.getVector(org, TransformMatrixInv, origin, 0);
	mobj.getVector(dir, TransformMatrixInv, direction, 1);

	/*find vectors for two edges sharing vert0*/
	vec3 edge1 = B - A;
	vec3 edge2 = C - A;

	/*begin calculating determinant*/
	vec3 pvec;
	cross(pvec,direction,edge2);

	/*If determinant is near zero, ray lies in the plane of the triangle*/
	double det = dot(edge1, pvec);
	
	//if (debug) printf("Debug: Triangle det=%f\n",det);
	
	/*If we want to cull the back faces of triangle*/
	if(TEST_CULL)
	{
		if(det < err)
			return false;

		/*Calculate the distance from vert0 to ray origin*/
		vec3 tvec = origin - A;

		/*Calculate the u param and test bounds*/
		u = dot(tvec,pvec);
		
		if(u < 0.0 || u > det)
			return false;

		/*Prepare to test v param*/
		vec3 qvec;
		cross(qvec, tvec, edge1);

		/*Calculate the v parameter and test bounds*/
		v = dot(direction,qvec);
		
		if(v < 0.0 || (u + v) > det)
			return false;

		/*Calculate t, scale parameters, ray intersects triangle*/
		double tempt = dot(edge2, qvec);
		double inv_det = 1.0/det;
		tempt *= inv_det;
		
		//if (debug) printf("Debug: Triangle intersection! tempt=%f, t=%f\n",tempt, intObj->t);
		
		if( tempt < intObj->t && tempt > 0.0001f)
		{
			intObj->t = tempt;
			intObj->mat = &mat;
			intObj->setPoint(origin, direction);
			//intObj->finalObject = this;
			getNormal(intObj->normal, intObj->Point);
			intObj->applyTransform(TransformMatrix);
			intObj->applyTransformToNormal(TransformMatrixInvTranspose);
		}
		
		return true;
	}
	//Not sure about the else part -- refer to the paper later
}

static float absolute(float num) {
	if (num < 0.0f) 
		 return -num;
	else return  num;
}
