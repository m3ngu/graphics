#include <stdio.h>

#if defined(__APPLE__)
	#include <GLUT/glut.h>
#else
	#include <gl/glut.h>
#endif

#include "main.h"
#include "Transform.h"
#include "tga.h"

int amount;

vec3 eye;
vec3 up;
vec3 lookAt;
bool updateLight0, updateLight1;
int w, h;
unsigned int nindices;
unsigned int *indices;
float xMax, yMax, zMax;

GLuint  TexID1,
		TexID2,
		TexIDSquirrel,
		TexIDShroom,
		TexIDWood,
		TexIDCatFur;      // Handles to our textures

objects objCat,
		objSquirrel,
		objShroom,
		objFlwr1,
		objFlwr2,
		objButt,
		objBench; //Stores all the data for the loaded object

float center[3] = { 2.0, 2.0, 0.3};

GLuint treeStump; //The tree stump
/********
 * Main *
 ********/
int main(int argc, char* argv[]) {

	// Load GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("HW3: Helper Scene");

	// Initialize
	init();

	// Register functions
	glutDisplayFunc(display);
	glutSpecialFunc(specialKey);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutReshapeWindow(600, 400);

	// Print instructions to console
	printHelp();

	// Start main loop
	glutMainLoop();

	return 0;
}
/********
 * Init *
 ********/
void init() {
	eye		= vec3(0,	-10	,	5	);
  //eye		= vec3(0,	20	,	80	);
	lookAt	= vec3(0,	0	,	5	);
	up		= vec3(0,	0	,	1	);
	amount = 5;
	xMax = 200.0f;
	yMax = 200.0f;
	zMax = 100.0f;
	
	// Load the textures
	TexID1			=	LoadTexture("Textures/Soil2.tga");
	TexID2			=	LoadTexture("Textures/sky.tga");
	TexIDShroom		=	LoadTexture("Textures/shroomTex13.tga");
	TexIDSquirrel	=	LoadTexture("Textures/Squirrel_diffuse.tga");
	TexIDWood		=	LoadTexture("Textures/wood256.tga");
	TexIDCatFur		=	LoadTexture("Textures/cat_fur.tga");

	LoadObject("Objects/cat.obj"			, objCat		, center , 0.1  );
	LoadObject("Objects/shroom.obj"			, objShroom		, center , 0.2	);
	LoadObject("Objects/squirrel.obj"		, objSquirrel	, center , 1	);
	LoadObject("Objects/toonflower1.obj"	, objFlwr1		, center , 0.1	);
	LoadObject("Objects/toonflower4.obj"	, objFlwr2		, center , 0.1	);
	LoadObject("Objects/BUTT_M_file.obj"	, objButt		, center , 0.75	);
	LoadObject("Objects/GardenSeat1.obj"	, objBench		, center , 0.03	);

	/*Making the wooden tree stump - hand drawn*/

	GLUquadricObj * cyl;
	cyl = gluNewQuadric() ;

	treeStump = glGenLists(1);
	glNewList(treeStump, GL_COMPILE);
		gluQuadricTexture(cyl, GL_TRUE);
		gluDisk(cyl,0,2,100,5);
		glTranslatef(0.0,0.0,-3.0);
		gluCylinder(cyl, 2, 2, 3, 100, 5);
		gluDisk(cyl,0,2,100,5);
		//glutSolidSphere(0.191, polySubdivisions, polySubdivisions);
	glEndList();

	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}
/***********
 * Display *
 ***********/
void display() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	gluLookAt(	   eye.x,    eye.y	,    eye.z,
				lookAt.x, lookAt.y	, lookAt.z,
				    up.x,     up.y	,     up.z);

	/*************\
	| Horizon/Sky |
	\*************/

	skybox(xMax, yMax, zMax);

	/*
	// Squirrel

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, objSquirrel.vertexdata);
	glDrawElements( GL_TRIANGLES, objSquirrel.nindices, GL_UNSIGNED_INT, objSquirrel.indices );
	glDisableClientState(GL_VERTEX_ARRAY);
	*/

	/*************\
	|   Shrooms   |
	\*************/

	float shroom_center[3] = {-5.0, 0.0, 4.0};

	glPushMatrix();
		glTranslated( shroom_center[0], shroom_center[1], shroom_center[2]);
		glRotated(90.0, 1.0, 0.0, 0.0);

		//Initialize and bind texture
		drawObject(objShroom, TexIDShroom, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslated( shroom_center[0]+7, shroom_center[1]+7, shroom_center[2]);
		glRotated(90.0, 0.0, 0.0, 1.0);
		glRotated(90.0, 1.0, 0.0, 0.0);

		//Initialize and bind texture
		drawObject(objShroom, TexIDShroom, 1);
	glPopMatrix();

	glPushMatrix();
		glTranslated( shroom_center[0]-15, shroom_center[1]+12, shroom_center[2]);
		glRotated(180.0, 0.0, 0.0, 1.0);
		glRotated(90.0, 1.0, 0.0, 0.0);

		//Initialize and bind texture
		drawObject(objShroom, TexIDShroom, 1);
	glPopMatrix();

	/*************\
	|   Flowers   |
	\*************/

	glPushMatrix();
		glTranslated(20,5,10);
		glRotated(90.0, 1.0, 0.0, 0.0);

		//Initialize and bind texture
		glColor3d(0.0, 0.25, 0.0);
		drawObject(objFlwr1, 0, 0);
	glPopMatrix();

	glPushMatrix();
		glTranslated(20,20,10);
		glRotated(90.0, 1.0, 0.0, 0.0);

		//Initialize and bind texture
		drawObject(objFlwr2, 0, 0);
	glPopMatrix();

	/*************\
	|    Butt     |
	\*************/

	glPushMatrix();
		glTranslated(2,-2 ,12);
		glRotated(75.0, 1.0, 0.0, 0.0);
		glRotated(-135.0, 0.0, 0.0, 1.0);

		//Initialize and bind texture
		glColor3d(1.0, 1.0, 0.0);
		drawObject(objButt, 0, 0);
	glPopMatrix();

	/*************\
	|    Bench    |
	\*************/

	glPushMatrix();
		glTranslated(-40 , 40 , 20);
		glRotated(45.0, 0.0, 0.0, 1.0);

		//Initialize and bind texture
		drawObject(objBench, TexIDWood, 1);
	glPopMatrix();

	/**************\
	|  Tree Stump  |
	\**************/

	glPushMatrix();

		glTranslated(50 , 50 , 20);
		glScaled(8, 8, 8);

		glEnable(GL_TEXTURE_2D);		// Enable Texture Mapping
			glBindTexture(GL_TEXTURE_2D, TexIDWood);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
			glCallList(treeStump);
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	/**************\
	|     Cat      |
	\**************/

	glPushMatrix();
		glTranslated(50 , 50 , 35);
		glRotated(90.0, 0.0, 0.0, 1.0);
		//glScaled(20, 20, 20);

		//Initialize and bind texture
		drawObject(objCat, TexIDCatFur, 1);
	glPopMatrix();

	glutSwapBuffers();
}
/**********
 * Skybox *
 **********/
void skybox(float width, float depth, float height) {

	float bottom = 0.0f;

	// Store the current matrix
    glPushMatrix();

	glTranslatef( eye.x, eye.y, bottom);

    //glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_BLEND);

	// Ground

	glBindTexture(GL_TEXTURE_2D, TexID1);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(  0.0,  0.0); glVertex3d(-width,-depth, bottom);
		glTexCoord2d( 10.0,  0.0); glVertex3d( width,-depth, bottom);
		glTexCoord2d( 10.0,  5.0); glVertex3d( width, depth, bottom);
		glTexCoord2d(  0.0,  5.0); glVertex3d(-width, depth, bottom);
	glEnd();

	// Sky - front

	glBindTexture(GL_TEXTURE_2D, TexID2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0,0.0); glVertex3d(-width, depth, bottom);
		glTexCoord2d(1.0,0.0); glVertex3d(-width, depth, height + eye.z);
		glTexCoord2d(1.0,1.0); glVertex3d( width, depth, height + eye.z);
		glTexCoord2d(0.0,1.0); glVertex3d( width, depth, bottom);
	glEnd();

	// Sky - left

	glBindTexture(GL_TEXTURE_2D, TexID2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0,0.0); glVertex3d(-width,  depth, bottom);
		glTexCoord2d(1.0,0.0); glVertex3d(-width,  depth, height + eye.z);
		glTexCoord2d(1.0,1.0); glVertex3d(-width, -depth, height + eye.z);
		glTexCoord2d(0.0,1.0); glVertex3d(-width, -depth, bottom);
	glEnd();

	// Sky - right

	glBindTexture(GL_TEXTURE_2D, TexID2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0,0.0); glVertex3d( width,  depth, bottom);
		glTexCoord2d(1.0,0.0); glVertex3d( width,  depth, height + eye.z);
		glTexCoord2d(1.0,1.0); glVertex3d( width, -depth, height + eye.z);
		glTexCoord2d(0.0,1.0); glVertex3d( width, -depth, bottom);
	glEnd();

	// Sky - back

	glBindTexture(GL_TEXTURE_2D, TexID2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(0.0,0.0); glVertex3d(-width, -depth, bottom);
		glTexCoord2d(1.0,0.0); glVertex3d(-width, -depth, height + eye.z);
		glTexCoord2d(1.0,1.0); glVertex3d( width, -depth, height + eye.z);
		glTexCoord2d(0.0,1.0); glVertex3d( width, -depth, bottom);
	glEnd();

	// Sky - top

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, TexID2);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin( GL_QUADS );
		glTexCoord2d(  0.0,  0.0); glVertex3d(-width,-depth, height + eye.z);
		glTexCoord2d(  1.0,  0.0); glVertex3d( width,-depth, height + eye.z);
		glTexCoord2d(  1.0,  1.0); glVertex3d( width, depth, height + eye.z);
		glTexCoord2d(  0.0,  1.0); glVertex3d(-width, depth, height + eye.z);
	glEnd();

	// Restore enable bits and matrix
    //glPopAttrib();

	glDisable(GL_TEXTURE_2D);

    glPopMatrix();
}
/***************
 * LoadTexture *
 ***************/
GLuint LoadTexture(char *TexName) {
	TGAImg Img;        // Image loader
	GLuint Texture;

	// Load our Texture
	if(Img.Load(TexName)!=IMG_OK)
		return -1;

	glGenTextures(1,&Texture);            // Allocate space for texture
	glBindTexture(GL_TEXTURE_2D,Texture); // Set our Tex handle as current

	// Create the texture
    if(Img.GetBPP()==24)
		glTexImage2D(GL_TEXTURE_2D,0,3,Img.GetWidth(),Img.GetHeight(),0,
					 GL_RGB,GL_UNSIGNED_BYTE,Img.GetImg());
    else if(Img.GetBPP()==32)
		glTexImage2D(GL_TEXTURE_2D,0,4,Img.GetWidth(),Img.GetHeight(),0,
					 GL_RGBA,GL_UNSIGNED_BYTE,Img.GetImg());
    else
		return -1;

	// Build Mipmaps (builds different versions of the picture for distances - looks better)
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, Img.GetWidth(), Img.GetHeight(), GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// Specify filtering and edge actions
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT); //GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

	return Texture;
}
/**************
 * DrawObject *
 **************/
void drawObject(objects obj, GLuint TexID, int texture) {

        //Initialize and bind texture
        if(texture > 0) {
               glEnable(GL_TEXTURE_2D);
               glBindTexture(GL_TEXTURE_2D, TexID);
               glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

               //Get the texture on the object
               glEnableClientState(GL_TEXTURE_COORD_ARRAY);
               glTexCoordPointer( 2, GL_FLOAT, 0, obj.texcoords );
        }

	    //Get the vertex pointer enabled
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, obj.vertexdata);

	    glDrawElements( GL_TRIANGLES, obj.nindices, GL_UNSIGNED_INT, obj.indices );

		//Disable the vertex, texture array and texture
		glDisableClientState(GL_VERTEX_ARRAY);
		if(texture > 0) {
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisable(GL_TEXTURE_2D);
		}

}
/***********
 * Reshape *
 ***********/
void reshape(int width, int height){
	w = width;
	h = height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90.0f, w/(float)h, 1.0f, 600.0f);
	glViewport(0, 0, w, h);
}
/*************
 * PrintHelp *
 *************/
void printHelp() {
	printf("press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n");
}
/************
 * Keyboard *
 ************/
void keyboard(unsigned char key, int x, int y) {
	switch(key) {
	case '+':
		amount++;
		printf("amount set to %d\n", amount);
		break;
	case '-':
		amount--;
		printf("amount set to %d\n", amount);
		break;
	case 'w':
		Transform::forward(static_cast<float>(amount), eye, lookAt, xMax, yMax, zMax);
		break;
	case 's':
		Transform::forward(static_cast<float>(-amount), eye, lookAt, xMax, yMax, zMax);
		break;
	case 'a':
		Transform::sideways(static_cast<float>(-amount), eye, lookAt, up, xMax, yMax);
		break;
	case 'd':
		Transform::sideways(static_cast<float>(amount), eye, lookAt, up, xMax, yMax);
		break;
	case 'c':
		Transform::straighten( eye, lookAt, up);
		break;
	case 'm':
		Transform::toggleMapView( eye, lookAt, up);
		break;
	}

	glutPostRedisplay();
}
/**************
 * SpecialKey *
 **************/
void specialKey(int key, int x, int y) {
	switch(key) {

		case GLUT_KEY_LEFT: //left
			Transform::left(static_cast<float>(amount), eye, lookAt,  up);
			break;
		case GLUT_KEY_UP: //up
			Transform::up(static_cast<float>(amount),  eye, lookAt,  up);
			break;
		case GLUT_KEY_RIGHT: //right
			Transform::left(static_cast<float>(-amount),  eye, lookAt,  up);
			break;
		case GLUT_KEY_DOWN: //down
			Transform::up(static_cast<float>(-amount), eye, lookAt,  up);
			break;
	}
	up.normalize();
	glutPostRedisplay();
}
