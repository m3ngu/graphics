
#include <iostream>
#include "Image.h"
#include "Scene.h"
#include "Light.h"

FILE *inputfile, *outputfile;;
Scene *s;
const double tmax = 20000.0;
const int max_depth = 6;

vec3 RayThruPixel(Camera * cam, float x, float y, int width, int height);
bool debugPixel(int x, int y);

bool refract(vec3& refractedRay, const vec3& normal, const vec3& incident, const double n) { 

	const double cosI = dot(normal, incident); 
	const double sinT2 = n * n * (1.0 - cosI * cosI); 
	if (sinT2 > 1.0) { 
		return false; 
	}
	refractedRay = n * incident - (n + sqrt(1.0 - sinT2)) * normal;
	return true;
}
/*
vec3 refraction(vec3& incident, vec3& normal, double dielectric)
{
	
	double cosTheta1 = dot(normal, vec3(incident.x, incident.y, incident.z) );
	double cosTheta2 = sqrt( 1 - (dielectric*dielectric)*( 1 - (cosTheta1*cosTheta1)) );
	if(cosTheta1 > 0)
	{
		vec3 vRefract = vec3(dielectric*incident.x, dielectric*incident.y, dielectric*incident.z);
		double temp = (dielectric*cosTheta1) - cosTheta2;
		vRefract = vec3(vRefract.x + (temp*normal.x), vRefract.y + (temp*normal.y), vRefract.z + (temp*normal.z) );
		vRefract.normalize();
		return vRefract;
	}
	else
	{
		vec3 vRefract = vec3(dielectric*incident.x, dielectric*incident.y, dielectric*incident.z);
		double temp = cosTheta2 + (dielectric*cosTheta1);
		vRefract = vec3(vRefract.x - (temp*normal.x), vRefract.y - (temp*normal.y), vRefract.z - (temp*normal.z) );
		vRefract.normalize();
		return vRefract;
	}
}
*/

Colors rayColor(vec3& origin, vec3& direction, const int depth, bool debug) {
	
	Colors finalColor = BLACK;
	
	if (depth == 0) return finalColor;
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//    Find closest intersection
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	int hits = 0;
	Intersect intersection = Intersect(origin, direction, tmax);
	
	for(int k = 0; k < s->objects.size(); k++) {
		
		/*
		 if (debug) {
		 printf("Debug: Shape: %i, type=%i, t=%f\n",k, s->objects[k]->type, intersection.t);
		 }
		 */
		if (s->objects[k]->intersect(origin, direction, &intersection, false)) {
			hits++;
		}
		
	}
	
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//    If ray doesn't intersect anything, return black
	// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	
	if (hits == 0) return finalColor;
		
	intersection.debug = debug;
	
	// =========================
	//    EMMISSION
	// =========================
	
	finalColor = s->ambient + intersection.mat->emission;
	
	// ~~~~~~~~~~~~~~~~~~~~~
	//    For each light
	// ~~~~~~~~~~~~~~~~~~~~~
	
	for (int i = 0; i < s->LightList.size(); i++) {
		
		Lights* light = s->LightList[i];
		
		// Calculate the light direction
		vec3 lightdirection;
		
		// Check if its a direction light or a positional light using dirflag = 1 
		// for directional and 0 for positional
		if (light->dirFlag == 0) {
			lightdirection =  (light->directionorpos) - (intersection.Point); // TODO: Check this
			lightdirection.normalize();
		} else {
			lightdirection = light->directionorpos;
		}
		
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//    Check whether the light source is behind the object
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		double cosTheta = dot(intersection.normal, lightdirection);
		
		if (intersection.debug) printf("Debug: light->directionorpos [%f, %f, %f]\n", (light->directionorpos).x, (light->directionorpos).y, (light->directionorpos).z);
		if (intersection.debug) printf("Debug: intersection.Point [%f, %f, %f]\n", (intersection.Point).x, (intersection.Point).y, (intersection.Point).z);
		if (intersection.debug) printf("Debug: lightdirection [%f, %f, %f]\n", lightdirection.x, lightdirection.y, lightdirection.z);
		if (intersection.debug) printf("Debug: cosTheta=%f\n", cosTheta);
		
		if (cosTheta <= 0) continue;
		
		// ~~~~~~~~~~~~~~~~~~~~~
		//    Check for Shadow
		// ~~~~~~~~~~~~~~~~~~~~~
		
		vec3 shadowOrigin = intersection.Point;
		vec3 shadowRay = lightdirection;
		Intersect *shadowIntersect = new Intersect();
		
		if (shadowIntersect->debug) printf("Debug: Org [%f, %f, %f]\n", shadowOrigin.x, shadowOrigin.y, shadowOrigin.z);
		if (shadowIntersect->debug) printf("Debug: Ray [%f, %f, %f]\n", shadowRay.x, shadowRay.y, shadowRay.z);
		
		
		// For point lights, we need to check if there is an object closer than the light source 
		if (light->dirFlag == 0) {
			shadowIntersect->t = nv_norm(lightdirection);
		} else {
			shadowIntersect->t = tmax;
		}
		
		if (shadowIntersect->debug) printf("Debug: t=%f\n", shadowIntersect->t);
		
		bool shadowed = false;
		
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		//    Check for each object until we find a shadowing object
		// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		for(int k = 0; k < s->objects.size(); k++) {
			
			shadowed = s->objects[k]->intersect(shadowOrigin, shadowRay, shadowIntersect, true);
			// Stop checking rest of the objects
			if (shadowed) {
				if (shadowIntersect->debug) printf("Debug: Light Source=%i shadowed by object=%i (type=%i)\n",i,k,s->objects[k]->type);
				break;
			}
		}
		// Go on to the next light source
		if (shadowed) {
			continue;
		}
		
		// =====================
		//    SPECULAR
		// =====================
		
		Colors specularColor;

		if ( ! (intersection.mat->specular == BLACK)) {
		
			//The viewing direction
			vec3 viewVec = origin - intersection.Point;
			viewVec.normalize();
			
			//The halfway vector (blinn-phong model)
			vec3 halfAngle = vec3((viewVec.x + lightdirection.x)/2.0,
								  (viewVec.y + lightdirection.y)/2.0,
								  (viewVec.z + lightdirection.z)/2.0);
			halfAngle.normalize();
			
			double specularity = (double) pow((double)(dot(intersection.normal, halfAngle)), (double)intersection.mat->shininess);
			specularity = specularity > 0 ? specularity : 0;
			specularColor = intersection.mat->specular * specularity;
		
		} else {
			specularColor = BLACK;
		}

		// =====================
		//    DIFFUSE
		// =====================
		
		Colors diffuseColor;
		
		if ( ! (intersection.mat->diffuse == BLACK)) {
			
			diffuseColor = intersection.mat->diffuse * cosTheta;
		
		} else {
			
			diffuseColor = BLACK;
		
		}

		// Add Light's Color x (Specular + Diffuse) to the overall color
		
		Colors lightColor = light->lightColor * ( specularColor + diffuseColor );
		finalColor = finalColor + lightColor;
		
		// =====================
		//    REFLECTION
		// =====================
		
		if (intersection.mat->reflect > 0.0f && depth > 0) {
			// Find reflection ray
			
			vec3 reflectedRay;
			reflect(reflectedRay, intersection.normal, direction);
			reflectedRay = -reflectedRay;
			
			vec3 reflectPoint = intersection.Point + (0.01f * reflectedRay);
			
			Colors reflectColor = rayColor(reflectPoint,
										   reflectedRay,
										   (depth-1),
										   debug);
			
			finalColor = finalColor +  reflectColor * (intersection.mat->reflect);
		}
		
		// =====================
		//    REFRACTION
		// =====================
		
		if (intersection.mat->translucency > 0.0f && depth > 0) {
			// Find reflection ray
			
			vec3 refractRay; // = refraction(direction, intersection.normal, intersection.mat->refract);
			
			if (refract(refractRay,
						intersection.normal,
						direction,
						intersection.mat->refract)) {
				
				vec3 refractPoint = intersection.Point + (0.01f * refractRay);
			
				Colors refractColor = rayColor(refractPoint,
											   refractRay,
											   (depth-1),
											   debug);
				
				finalColor = finalColor +  refractColor * (intersection.mat->translucency);
			}
			/*
			Colors refractColor = rayColor(intersection.Point,
										   refractRay,
										   (depth-1),
										   debug);
			
			finalColor = finalColor +  refractColor * (intersection.mat->translucency);
			 */
		}
		
		
	}
	
	if (intersection.debug) {
		printf("Debug: finalColor r=%f, g=%f, b=%f\n", finalColor.r, finalColor.g, finalColor.b);
	}
	
	return finalColor;
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
	
	//triangle tri[2];
	//tri[0] = triangle(vec3(-1,-1,0), vec3(1,-1,0), vec3(1,1,0));
	//tri[1] = triangle(vec3(-1,-1,0), vec3(1,1,0), vec3(-1,1,0));
	
	int y,x;
	Colors finalColor;
	
	vec3 origin = *(cam->getEye());
	
	for (y = 0 ; y < s->getSizeY() ; y++) {
		for (x = 0 ; x < s->getSizeX() ; x++) {
			
			Colors pixelColor = BLACK;
			
			for (float fragmentx = float(x) ; fragmentx < x + 1.0f; fragmentx += 0.5f ) {
			    for (float fragmenty = float(y) ; fragmenty < y + 1.0f; fragmenty += 0.5f )
			    {
				    
					float sampleRatio = 0.25f;
				    
                    vec3 ray = RayThruPixel(cam, fragmentx, fragmenty, s->getSizeX(), s->getSizeY()) ; 
					
					if (debugPixel(x,y)) {
						printf("Debug: x=%i, y=%i\n", x, y);
					}
					
					Colors temp = rayColor(origin, ray, 6, debugPixel(x,y));
					
                    // pseudo photo exposure
                    float exposure = -1.00f; // random exposure value. TODO : determine a good value automatically
	                temp.b = (1.0f - expf(temp.b * exposure));
	                temp.r =  (1.0f - expf(temp.r * exposure));
	                temp.g = (1.0f - expf(temp.g * exposure));
					
	                pixelColor = pixelColor + (temp * sampleRatio);
					
					if (pixelColor.r > 1.0) pixelColor.r = 1.0;
					if (pixelColor.g > 1.0) pixelColor.g = 1.0;
					if (pixelColor.b > 1.0) pixelColor.b = 1.0;
					
					outImg->setPixel(y, x, (int)(pixelColor.r * 255.0f),
									 (int)(pixelColor.g * 255.0f),
									 (int)(pixelColor.b * 255.0f));
			    }
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
		printf("Status: Rendering line %i of %i...\n", y+1, s->getSizeY());
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

vec3 RayThruPixel(Camera * cam, float x, float y, int width, int height) {
	
	vec3 * u = cam->getU();
	vec3 * v = cam->getV();
	vec3 * w = cam->getW();
	
	float fovx = cam->getFovX();
	float fovy = cam->getFovY();
	
	
	//vec3 * eye = cam->getEye();
	
	float alpha = tan(fovx/2.0f) * (( x - ((float)width/2.0f)) / ((float)width/2.0f)); 
	float beta = tan(fovy/2.0f) * ((((float)height/2.0f) - y) / ((float)height/2.0f));
	
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
	//if (x == 90 && y == 105) return true;
	if (x == 140 && y == 150) return true;
	return false;
}
