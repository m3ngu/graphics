
#include <iostream>
#include "Scene.h"
#include "Image.h"
#include "Light.h"
#include "Intersection.h"

FILE *inputfile, *outputfile;;
Scene *s;

vec3 RayThruPixel(Camera * cam, int x, int y, int width, int height);
bool debugPixel(int x, int y);

void getVector(vec3 givenVector, mat4 transformMatrix, vec3 & outputVector, int dirflag) {
	vec4 homoVector;
	if(dirflag == 0)
		homoVector = vec4(givenVector.x,givenVector.y,givenVector.z,1);
	else
		homoVector = vec4(givenVector.x,givenVector.y,givenVector.z,0);

	vec4 newVector = transformMatrix * homoVector;
	outputVector = vec3(newVector.x,newVector.y,newVector.z);
}

//gloAmb is defined in Scene.cpp which is a LightColor object
//void rayColor(Intersect *intObject, int depth, LightColor& finalColor) {
//	//Convert the emission property into a LightColor object with r,g and b
//	LightColor objectMatEmission = LightColor(intObject->mat->emission[0], intObject->mat->emission[1], intObject->mat->emission[2]);
//	finalColor = gloAmb + objectMatEmission;
//	for(unsigned int i = 0; i < pointLightList.size(); i++)
//	{
//		//Get the current PointLight variable
//		PointLight light = pointLightList[i];
//		//Calculate the light direction
//		vec3 lightdirection = (light.pos)-intObject->Point;
//		lightdirection.normalize();
//		//Calculate the halfway vector for specular using blinnphong model
//		vec3 halfway = (intObject->rayOrigin + lightdirection);
//		halfway.normalize();
//
//		//If the object is not in shade : TODO.. then find the color
//
//		float d = distanceBetweenPoints(light.pos,hit.intersection.point);
//
//		float attenuatingfact = calculateAttenuation(pointLightList[i].attenuationcoeffs[0],pointLightList[i].attenuationcoeffs[1],pointLightList[i].attenuationcoeffs[2],d);
//		RGBA diffuse_component;
//		diffuse_component.r = min((attenuatingfact * hit.intersection.obj.mat.material_diffuse.r * max(dot(lightdirection,hit.normal),0.005)),1);
//		diffuse_component.g = min((attenuatingfact * hit.intersection.obj.mat.material_diffuse.g * max(dot(lightdirection,hit.normal),0.005)),1);
//		diffuse_component.b = min((attenuatingfact * hit.intersection.obj.mat.material_diffuse.b * max(dot(lightdirection,hit.normal),0.005)),1);
//		diffuse_component.a = min((attenuatingfact * hit.intersection.obj.mat.material_diffuse.a * max(dot(lightdirection,hit.normal),0.005)),1);
//		RGBA specular_component;
//		specular_component.r = min((hit.intersection.obj.mat.material_specular.r * pow(dot(hit.normal, halfway),hit.intersection.obj.mat.shininess)),1);
//		specular_component.g = min((hit.intersection.obj.mat.material_specular.g * pow(dot(hit.normal, halfway),hit.intersection.obj.mat.shininess)),1);
//		specular_component.b = min((hit.intersection.obj.mat.material_specular.b * pow(dot(hit.normal, halfway),hit.intersection.obj.mat.shininess)),1);
//		specular_component.a = min((hit.intersection.obj.mat.material_specular.a * pow(dot(hit.normal, halfway),hit.intersection.obj.mat.shininess)),1);
//		RGBA color = pointLightList[i].color * (diffuse_component + specular_component);
//		ray.color = ray.color + color;
//	}
//}
int main (int argc, char * const argv[]) {
    
	if (argc != 2) {
		fprintf(stderr, "Usage: hw4_proj filename\n") ;
		exit(1) ;
	}
	
	// Load scene information from text file
	// -- Read and parse file
	
	assert (inputfile = fopen(argv[1], "rt"));
	
	s = new Scene();
	
	s->parsefile(inputfile);
	
	Camera * cam = s->getCam();
	
	//printf("Debug: Camera fovx=%f fovy=%f\n", cam->getFovX(), cam->getFovY());
	
	// Build scene
	// -- Apply transformations
	
	// Main Loop 
	// -- Shoot eye ray through each pixel 
	// ---- Loop through every object 
	// ------ Test Intersection 
	// ------ If visible ﬁnd Color 
	// ------ Cast Shadow Rays to each light Color += reﬂectivity * color of reﬂected ray 
	
	Image *outImg = new Image();
	outImg->setColors(255);
	outImg->setSize(s->getSizeY(), s->getSizeX()); // Rows (=Y), Columns (=X)
	
	triangle tri[2];
	tri[0] = triangle(vec3(-1,-1,0), vec3(1,-1,0), vec3(1,1,0));
	tri[1] = triangle(vec3(-1,-1,0), vec3(1,1,0), vec3(-1,1,0));
	
	int y,x;
	const double tmax = 20000.0;
	Intersect *intersection = new Intersect();
	
	for (y = 0 ; y < s->getSizeY() ; y++) {
		for (x = 0 ; x < s->getSizeX() ; x++) {
			vec3 ray = RayThruPixel(cam, x, y, s->getSizeX(), s->getSizeY()) ; 
			
			vec3 origin = *(cam->getEye());
			//vec3 origin = vec3(0,0,0);
			
			if (debugPixel(x,y)) {
				printf("Debug: x=%i, y=%i\n", x, y);
			}

			// Reset intersection object
			int hits = 0;
			intersection->t = tmax;
			intersection->rayOrigin = origin;
			intersection->rayDirection = ray;

			for(int k = 0; k < s->objects.size(); k++) {
				
				s->objects[k]->debug = debugPixel(x,y);
				
				if (debugPixel(x,y)) {
					printf("Debug: Shape: %i, type=%i, t=%f\n",k, s->objects[k]->type, intersection->t);
				}
				mat4 transform = s->objects[k]->TransformMatrix;
				mat4 inverseTransform = invert(inverseTransform,transform);
				vec3 neworg, newdir;
				getVector(origin,inverseTransform,neworg,0);
				getVector(ray,inverseTransform,newdir,1);

				if (s->objects[k]->intersect(neworg, newdir, intersection))
				{
					hits++;
					vec3 pt;
					getVector(intersection->Point,transform,pt,0);
				}
				
			}
			
			if (hits > 0) {
				//printf("Comes here\n");
				LightColor finalColor = LightColor(1.0,1.0,1.0,1.0);
				

				//rayColor(intersection, 10, finalColor);
				/*
				Material* m = intersection->mat;
				if(m != NULL)
				{
					float r = m->diffuse[0] * 255.0f;
					float g = m->diffuse[1] * 255.0f;
					float b = m->diffuse[2] * 255.0f;
				}
				else
				{
					float r = 255.0;
					float g = 255.0;
					float b = 255.0;
				}
				
				if ((x==s->getSizeX()/2) && (y==s->getSizeY()/2)) {
					printf("Debug: diffuse r=%f g=%f b=%f\n",
						r,
						g,
						b);
					
					printf("Debug: diffuse r=%i g=%i b=%i\n",
						(int)r,
						(int)g,
						(int)b);
						
				}
				*/
				outImg->setPixel(y, x, 255.0, 255.0, 255.0);
				//outImg->setPixel(y, x, (int)r, (int)g, (int)b);
			} else {
				outImg->setPixel(y, x, 0, 0, 0);
			}

			
			
			//printf("Debug: Origin x=%i y=%i\n", x, y);
			/*
			for (int i=0; i < 2 ; i++)
			{
				if (tri[i].intersect(origin, ray)) {
					outImg->setPixel(y, x, 255, 0, 0);
				}
			}
			*/
			//Intersection hit = Intersect (ray, scene) ; 
			//image[i][j] = FindColor(hit) ; 
		}
	}
	
	
	// Write Image
	
	char *outFileName = s->getoutFileName();
	
	printf("Status: Writing output image to file: %s\n", outFileName);
	if (outImg->writeImage(outFileName) != 0) {
		printf("Error: Cannot write to file: %s\n", outFileName);
		return -1;
	}

	delete outImg;
	
    fprintf(stdout, "Finished!\n");
    return 0;
}

vec3 RayThruPixel(Camera * cam, int x, int y, int width, int height) {
	
	vec3 * u = cam->getU();
	vec3 * v = cam->getV();
	vec3 * w = cam->getW();
	
	float fovx = cam->getFovX();
	float fovy = cam->getFovY();
	
	
	//vec3 * eye = cam->getEye();
	
	float alpha = tan(fovx/2.0f) * (( (float)x - ((float)width/2.0f)) / ((float)width/2.0f)); 
	float beta = tan(fovy/2.0f) * ((((float)height/2.0f) - (float)y) / ((float)height/2.0f));
	
	vec3 dir = (alpha * *u) + (beta * *v) - *w;
	dir.normalize();
	
	/*
	if ((x==width/2-100) && (y==height/2-100)) {
		printf("Debug: fovy=%f, fovx=%f\n", fovy, fovx);
		printf("Debug: alpha=%f, beta=%f\n", alpha, beta);
		printf("Debug: Ray x=%f y=%f z=%f\n", dir.x, dir.y, dir.z);
	}
	*/
	
	return dir; // + eye
	
}

bool debugPixel(int x, int y) {
	if (x == 320 && y == 277) return true;
	if (x == 263 && y == 277) return true;
	return false;
}
