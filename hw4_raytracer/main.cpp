
#include <iostream>
#include "Scene.h"
#include "Image.h"

FILE *inputfile, *outputfile;;
Scene *s;

vec3 RayThruPixel(Camera * cam, int x, int y, int width, int height);
bool debugPixel(int x, int y);

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
				
				if (s->objects[k]->intersect(origin, ray, intersection)) hits++;
				
			}
			
			if (hits > 0) {
				Material* m = intersection->mat;
					
				float r = m->diffuse[0] * 255.0f;
				float g = m->diffuse[1] * 255.0f;
				float b = m->diffuse[2] * 255.0f;
				/*
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
				
				outImg->setPixel(y, x, (int)r, (int)g, (int)b);
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