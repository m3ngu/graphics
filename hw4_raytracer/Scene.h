/*
 *  Scene.h
 */

#include <iostream>
#include <math.h>
#include "nv/nv_math.h"
#include "nv/nv_algebra.h"
#include "Camera.h"
#include "objects.h"
#include "Stack.h"
#include "MatrixTransformation.h"
#include<vector>

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
  
	StackObject *stck;
	// Constructor / Deconstructor
	Scene ();
	~Scene ();

	// List of objects (i.e. shapes/primitives) in the scene
    std::vector<obj*> objects;

	mat4 Scene::lookAt(Camera *cam);
    void parsefile (FILE *fp);
	
	// Accessor functions
	char * getoutFileName() { return outFileName; };
	int getSizeX () { return sizex; };
	int getSizeY () { return sizey; };
	Camera * getCam() { return cam; };
};
