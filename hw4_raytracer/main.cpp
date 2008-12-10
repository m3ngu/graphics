
#include <iostream>
#include "Image.h"
#include "Scene.h"
#include "Light.h"

FILE *inputfile, *outputfile;;
Scene *s;

vec3 RayThruPixel(Camera * cam, int x, int y, int width, int height);
bool debugPixel(int x, int y);

//gloAmb is defined in Scene.cpp which is a LightColor object
void rayColor(vec3 eyePoint, Intersect *intObject, int depth, Colors& finalColor) {
	
	// =========================
	//    AMBIENT + EMMISSION
	// =========================
	
	finalColor = s->ambient + intObject->mat->emission;
	
	// ~~~~~~~~~~~~~~~~~~~~~
	//    For each light
	// ~~~~~~~~~~~~~~~~~~~~~
	
	for (int i = 0; i < s->LightList.size(); i++) {
		
		Lights* light = s->LightList[i];
		
		// Calculate the light direction
		vec3 lightdirection;
		
		// Check if its a direction light or a positional light using dirflag = 1 
		// for directional and 0 for positional
		if(light->dirFlag == 0) {			
			lightdirection =  (light->directionorpos) - (intObject->Point); // TODO: Check this
			lightdirection.normalize();
		} else {
			lightdirection = light->directionorpos;
		}
		
		// Check whether the light source is behind the object
		double cosTheta = dot(intObject->normal, lightdirection);
		
		if (intObject->debug) {
			printf("Debug: cosTheta=%f\n", cosTheta);
		}
		
		
		if (cosTheta <= 0) continue;
		
		// Check for Shadow
		
		
		// =====================
		//    SPECULAR
		// =====================

		//The viewing direction
		vec3 viewVec = eyePoint - intObject->Point;
		viewVec.normalize();
		
		//The halfway vector (blinn-phong model)
		vec3 halfAngle = vec3((viewVec.x + lightdirection.x)/2.0,
							  (viewVec.y + lightdirection.y)/2.0,
							  (viewVec.z + lightdirection.z)/2.0);
		halfAngle.normalize();
		
		double specularity = (double) pow((double)(dot(intObject->normal, halfAngle)), (double)intObject->mat->shininess);
		specularity = specularity > 0 ? specularity : 0;
		Colors specularColor = intObject->mat->specular * specularity;
		
		// =====================
		//    DIFFUSE
		// =====================
		
		Colors diffuseColor = intObject->mat->diffuse * cosTheta;
		
		// Add Light's Color x (Specular + Diffuse) to the overall color
		
		Colors lightColor = light->lightColor * ( specularColor + diffuseColor );
		finalColor = finalColor + lightColor;
	}
	
	if (intObject->debug) {
		printf("Debug: finalColor r=%f, g=%f, b=%f\n", finalColor.r, finalColor.g, finalColor.b);
	}
}

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
	Colors finalColor;
	
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
				/*
				if (debugPixel(x,y)) {
					printf("Debug: Shape: %i, type=%i, t=%f\n",k, s->objects[k]->type, intersection->t);
				}
				*/
				if (s->objects[k]->intersect(origin, ray, intersection)) {
					hits++;
				}
				
			}
			
			if (hits > 0) {
				//printf("Comes here\n");
				intersection->debug = debugPixel(x,y);
				rayColor(intersection->rayOrigin, intersection, 10, finalColor);
				
				//printf("Debug: R: %f, G: %f, B:%f\n", finalColor.r, finalColor.g, finalColor.b);
				
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
				
				if (finalColor.r > 1.0) finalColor.r = 1.0;
				if (finalColor.g > 1.0) finalColor.g = 1.0;
				if (finalColor.b > 1.0) finalColor.b = 1.0;
				
				outImg->setPixel(y, x, (int)(finalColor.r * 255.0f),
									   (int)(finalColor.g * 255.0f),
									   (int)(finalColor.b * 255.0f));
				
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
	if (x == 218 && y == 121) return true;
	//if (x == 263 && y == 277) return true;
	return false;
}
