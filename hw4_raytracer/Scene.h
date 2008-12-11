/*
 *  Scene.h
 */

#include <iostream>
#include <math.h>
#include "Camera.h"
#include "objects.h" // Includes intersection -> Material -> Light
#include "Stack.h"
#include "MatrixTransformation.h"
#include <vector>

struct Vertex {
  double pos[3] ;
};

struct VertexNormal {
  double pos[3] ;
  double normal[3] ;
};

class Scene {

	int sizex, sizey;
	
	char outFileName[100];
	
    Vertex *vert;
	VertexNormal *vertnorm;
	
	int maxverts, maxvertnorms;
	int curvert, curvertnorm;

	int maxdepth;
	int lightnum;
	
	Camera *cam;
	mat4 modelViewMatrix;

  public:
	Attenuation currAttenuation;
	cubemap cm;
	StackObject *stck;
	// Constructor / Deconstructor
	Scene ();
	~Scene ();

	// List of objects (i.e. shapes/primitives) in the scene
    std::vector<obj*> objects;
	std::vector<Lights*> LightList;
	
	Colors ambient;  // We implement ambient as color

	mat4 Scene::lookAt(Camera *cam);
    void parsefile (FILE *fp);
	
	// Accessor functions
	char * getoutFileName() { return outFileName; };
	int getSizeX () { return sizex; };
	int getSizeY () { return sizey; };
	Camera * getCam() { return cam; };
};
