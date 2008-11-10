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

/*For animation of the ants*/
GLdouble antLoc = 0.0;
GLint animate = 0;
GLfloat ant_x_pos = 0.0;
GLfloat ant_y_pos = 20.0;
GLfloat ant_z_pos = 0.0;
int ant_counter = 1;
int ant_flag = 0;
int pause_flag = 0;

const GLfloat PI = 3.1415926f;
const GLfloat RAD_TO_DEG = 180.0 / PI;
const GLfloat TWO_PI = 2.0 * PI;
/******************************/
/*For animation of grass*/
float grassAngle = 0.0;
int grass_flag = 0;
int grass_counter = 1;
/************************/

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
		objPebble,
		objAnt,
		objBench; //Stores all the data for the loaded object

float center[3] = { 2.0, 2.0, 0.3};

/*Random points
**for pebbles**
***************/

float pebbleArray[100][3];
float pebbleScaleArray[100][3];

/*Random points
*for grass blade
***************/
float grassArray[150][3];


/******
*Nurbs*
******/
GLfloat ctlpoints[4][4][3];
GLUnurbsObj *theNurb;

GLuint treeStump; //The tree stump
GLuint pebble; //The pebble
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

void grassBlade(float x, float y, float height, float anglex, float anglez) 
{
	float ow = 0.25f;
	float od = 0.10f;
	float oh = height;
	vec3* v1 = new vec3(0.0f, 0.0f, oh  );
	vec3* v2 = new vec3(-ow , -od , 0.0f);
	vec3* v3 = new vec3( ow , -od , 0.0f);
	vec3* v4 = new vec3( ow , -od , 0.0f);
	vec3* v5 = new vec3( ow ,  od , 0.0f);
	vec3* v6 = new vec3( ow ,  od , 0.0f);
	vec3* v7 = new vec3(-ow ,  od , 0.0f);
	vec3* v8 = new vec3(-ow ,  od , 0.0f);
	vec3* v9 = new vec3(-ow , -od , 0.0f);
	vec3 n1; cross(n1, *v2 - *v1, *v3 - *v1); n1.normalize();
	vec3 n2; cross(n2, *v4 - *v1, *v5 - *v1); n2.normalize();
	vec3 n3; cross(n3, *v6 - *v1, *v7 - *v1); n3.normalize();
	vec3 n4; cross(n4, *v8 - *v1, *v9 - *v1); n4.normalize();
	glPushMatrix();

	glTranslatef(x, y, 0.0f);
	glRotatef(anglez, 0.0f, 0.0f, 1.0f);
	glRotatef(anglex, 1.0f, 0.0f, 0.0f);
	//glEnable(GL_COLOR_MATERIAL);
	//float kd[3] = {0.140397f, 0.283725f, 0.0654061f};
	//float kd[3] = {0.430477f, 0.464616f, 0.209123f};
	float kd[3] = {0.0f, 0.75f, 0.0f};
	float ks[3] = {0.25f,0.25f,0.25f};

	glMaterialfv(GL_FRONT, GL_DIFFUSE, kd);
	glMaterialfv(GL_FRONT, GL_AMBIENT, kd);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ks);
	glMaterialf(GL_FRONT, GL_SHININESS, 120.0f); // / 1000.0f * 128.0f

	glBegin(GL_TRIANGLES); // Start Drawing The Pyramid
	glColor3f(0.0f,1.0f,0.0f); // Green
	glNormal3f( n1.x, n1.y, n1.z);

	glVertex3f( v1->x, v1->y, v1->z); // Top Of Triangle (Front)
	glVertex3f( v2->x, v2->y, v2->z); // Left Of Triangle (Front)
	glVertex3f( v3->x, v3->y, v3->z); // Right Of Triangle (Front)

	glNormal3f( n2.x, n2.y, n2.z);
	glVertex3f( v1->x, v1->y, v1->z); // Top Of Triangle (Right)
	glVertex3f( v4->x, v4->y, v4->z); // Left Of Triangle (Right)
	glVertex3f( v5->x, v5->y, v5->z); // Right Of Triangle (Right)

	glNormal3f( n3.x, n3.y, n3.z);
	glVertex3f( v1->x, v1->y, v1->z); // Top Of Triangle (Back)
	glVertex3f( v6->x, v6->y, v6->z); // Left Of Triangle (Back)
	glVertex3f( v7->x, v7->y, v7->z); // Right Of Triangle (Back)
	glNormal3f( n4.x, n4.y, n4.z);

	glVertex3f( v1->x, v1->y, v1->z); // Top Of Triangle (Left)
	glVertex3f( v8->x, v8->y, v8->z); // Left Of Triangle (Left)
	glVertex3f( v9->x, v9->y, v9->z); // Right Of Triangle (Left)
	glEnd();

	//glDisable(GL_COLOR_MATERIAL);

glPopMatrix();
}


/************************
*Random Object Generator*
*************************/

void randomPoints(float randArray[][3], int xStart, int xEnd, int yStart, int yEnd, int zStart, int zEnd, 
				  int numberOfPoints, bool symm)
{
	for(int i = 0; i < numberOfPoints; i++) 
	{
		float xrandom = 0.0, yrandom = 0.0, zrandom = 0.0;
		int xLimit = 0, yLimit = 0, zLimit = 0;
		
		if(xStart > xEnd)
			xLimit = xStart - xEnd;
		else
			xLimit = xEnd - xStart;
		if(yStart > yEnd)
			yLimit = yStart - yEnd;
		else
			yLimit = yEnd - yStart;
		if(zStart > zEnd)
			zLimit = zStart - zEnd;
		else
			zLimit = zEnd - zStart;

		if(symm == true)
		{
			if(xLimit != 0)
				xrandom = xStart + float(rand()%xLimit)+((float)rand()/10000);
			if(yLimit != 0)
				yrandom = yStart + float(rand()%yLimit)+((float)rand()/10000);
			if(zLimit != 0)
				 zrandom = zStart + float(rand()%zLimit)+((float)rand()/10000);
		
			if(rand()%3 == 0)
				xrandom = -xrandom;
			if(rand()%3 == 0)
				yrandom = -yrandom;
			if(rand()%3 == 0)
				zrandom = -zrandom;
		}
		else
		{
			if(xLimit != 0)
				xrandom = xStart + float(rand()%xLimit);
			if(yLimit != 0)
				yrandom = yStart + float(rand()%yLimit);
			if(zLimit != 0)
				 zrandom = zStart + float(rand()%zLimit);
		}
		randArray[i][0] = xrandom;
		randArray[i][1] = yrandom;
		randArray[i][2] = zrandom;
	}
}

/******
*Nurbs*
*******/

/*Initialize*/
void init_surface(void)
{
    int u, v;
    for (u = 0; u < 4; u++)
	{
		for (v = 0; v < 4; v++) 
		{
			ctlpoints[u][v][0] = u;
			ctlpoints[u][v][1] = (u + v);
			if (u == 1 && v == 2)
			{
				ctlpoints[u][v][2] = 4.5;
				ctlpoints[u][v][2] = 5;
			}
		}
	}
}
/*Display Nurbs*/

void drawNurbs(GLUnurbsObj *theNurbObj, GLfloat knots[], GLuint texture, GLfloat ctlpoints[][4][3])
{
	gluBeginSurface(theNurbObj);
		gluNurbsSurface(theNurbObj, 8, knots,
            8, knots,
            4 * 3,
            3,
            &ctlpoints[0][0][0], 
            4, 4, GL_MAP2_TEXTURE_COORD_2);

		gluNurbsSurface(theNurbObj, 8, knots,
            8, knots,
            4 * 3,
            3,
            &ctlpoints[0][0][0], 
            4, 4, GL_MAP2_NORMAL);

		if(texture > 0) {
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, texture);
		}
        gluNurbsSurface(theNurbObj, 
            8, knots,
            8, knots,
            4 * 3,
            3,
            &ctlpoints[0][0][0], 
            4, 4,
            GL_MAP2_VERTEX_3);
        gluEndSurface(theNurbObj);
		
		if(texture > 0)
			glDisable(GL_TEXTURE_2D);
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

	/*LoadObject("Objects/cat.obj"			, objCat		, center , 0.1  );
	LoadObject("Objects/shroom.obj"			, objShroom		, center , 0.2	);
	LoadObject("Objects/squirrel.obj"		, objSquirrel	, center , 1	);
	LoadObject("Objects/toonflower1.obj"	, objFlwr1		, center , 0.1	);
	LoadObject("Objects/toonflower4.obj"	, objFlwr2		, center , 0.1	);
	LoadObject("Objects/BUTT_M_file.obj"	, objButt		, center , 0.75	);
	LoadObject("Objects/GardenSeat1.obj"	, objBench		, center , 0.03	);*/

	//LoadObject("Objects/pebble1.obj" ,objPebble, center , 0.2);
	//LoadObject("Objects/Ant.obj" ,objAnt, center , 0.2);
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

	/*Make pebbles*/
	GLUquadricObj *sphere = gluNewQuadric();
	pebble = glGenLists(2);
	glNewList(pebble, GL_COMPILE);
		gluSphere(sphere,0.5,15,15);
	glEndList();

	/*Initialize random pebble position*/
	//randomPoints(pebbleArray,100,50,0,100,true);
	randomPoints(pebbleArray,0,100,0,50,0,0,100,true);

	/*Initialize random grass position*/
	randomPoints(grassArray,5,50,10,50,0,0,150,true);

	/*Make ant*/
	glutWireSphere(2,15,15);


	/*Nurb curves*/
	init_surface();
    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);

	glEnable(GL_DEPTH_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnable(GL_TEXTURE_2D);

	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
}

//printf("ant_counter is %d",ant_counter);
void animation(void)
{
	/*antLoc = antLoc + 0.005 ;
	if (antLoc > 0.5) 
		antLoc = -0.5 ;*/
	//printf("grass angle is %f\n",grassAngle);
	if(grassAngle > 25.0)
	{
		grass_flag = 1;
	}
	if(grass_flag == 1)
	{
		grassAngle -= 0.2;
		if(grassAngle <= 0)
			grass_flag = 2;
	}
	else if(grass_flag == 2)
	{
		/*grassAngle -= 0.2;
		if(grassAngle < -25.0)
			grass_flag = 0;*/
		if(grass_counter%50 == 0)
			grass_flag = 0;
		else
			grass_counter++;
	}
	else
	{
		grass_counter = 1;
		grassAngle += 0.2f;
	}
	/*Ant Animation*/
	if(ant_counter%50 == 0)
	{
		antLoc += 0.005*8;
		pause_flag = 1;
	}
	//printf("the antLoc is %f and the if stmt is %f\n",antLoc,0.005*4);
	if((antLoc >= 0.005*10 && antLoc <= 0.005*12) || (antLoc >= 0.005*70 && antLoc <= 0.005*72) 
		|| (antLoc >= 0.005*300 && antLoc <= 0.005*302))
	{
		ant_counter++;
		ant_flag = 1;
	}
	if(pause_flag == 1)
	{
		antLoc -= 0.005*8;
		pause_flag = 0;
		ant_counter = 1;
		ant_flag = 0;
	}
	if(ant_flag == 0)
	{
		ant_x_pos =  20.0 * sin(antLoc);
		ant_y_pos =  20.0 * cos(antLoc);
		antLoc += 0.005f;
		if (antLoc > TWO_PI) 
			antLoc -= TWO_PI;
	}

	glutPostRedisplay() ; 
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

	///*************\
	//|   Shrooms   |
	//\*************/

	//float shroom_center[3] = {-5.0, 0.0, 4.0};

	//glPushMatrix();
	//	glTranslated( shroom_center[0], shroom_center[1], shroom_center[2]);
	//	glRotated(90.0, 1.0, 0.0, 0.0);

	//	//Initialize and bind texture
	//	drawObject(objShroom, TexIDShroom, 1);
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslated( shroom_center[0]+7, shroom_center[1]+7, shroom_center[2]);
	//	glRotated(90.0, 0.0, 0.0, 1.0);
	//	glRotated(90.0, 1.0, 0.0, 0.0);

	//	//Initialize and bind texture
	//	drawObject(objShroom, TexIDShroom, 1);
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslated( shroom_center[0]-15, shroom_center[1]+12, shroom_center[2]);
	//	glRotated(180.0, 0.0, 0.0, 1.0);
	//	glRotated(90.0, 1.0, 0.0, 0.0);

	//	//Initialize and bind texture
	//	drawObject(objShroom, TexIDShroom, 1);
	//glPopMatrix();

	///*************\
	//|   Flowers   |
	//\*************/

	//glPushMatrix();
	//	glTranslated(20,5,10);
	//	glRotated(90.0, 1.0, 0.0, 0.0);

	//	//Initialize and bind texture
	//	glColor3d(0.0, 0.25, 0.0);
	//	drawObject(objFlwr1, 0, 0);
	//glPopMatrix();

	//glPushMatrix();
	//	glTranslated(20,20,10);
	//	glRotated(90.0, 1.0, 0.0, 0.0);

	//	//Initialize and bind texture
	//	drawObject(objFlwr2, 0, 0);
	//glPopMatrix();

	///*************\
	//|    Butt     |
	//\*************/

	//glPushMatrix();
	//	glTranslated(2,-2 ,12);
	//	glRotated(75.0, 1.0, 0.0, 0.0);
	//	glRotated(-135.0, 0.0, 0.0, 1.0);

	//	//Initialize and bind texture
	//	glColor3d(1.0, 1.0, 0.0);
	//	drawObject(objButt, 0, 0);
	//glPopMatrix();

	///*************\
	//|    Bench    |
	//\*************/

	//glPushMatrix();
	//	glTranslated(-40 , 40 , 20);
	//	glRotated(45.0, 0.0, 0.0, 1.0);

	//	//Initialize and bind texture
	//	drawObject(objBench, TexIDWood, 1);
	//glPopMatrix();

	///**************\
	//|  Tree Stump  |
	//\**************/

	//glPushMatrix();

	//	glTranslated(50 , 50 , 20);
	//	glScaled(8, 8, 8);

	//	glEnable(GL_TEXTURE_2D);		// Enable Texture Mapping
	//		glBindTexture(GL_TEXTURE_2D, TexIDWood);
	//		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//		glCallList(treeStump);
	//	glDisable(GL_TEXTURE_2D);
	//glPopMatrix();

	///**************\
	//|     Cat      |
	//\**************/

	//glPushMatrix();
	//	glTranslated(50 , 50 , 35);
	//	glRotated(90.0, 0.0, 0.0, 1.0);
	//	//glScaled(20, 20, 20);

	//	//Initialize and bind texture
	//	drawObject(objCat, TexIDCatFur, 1);
	//glPopMatrix();

	/***************
	*Ants Animation*
	***************/
	glPushMatrix();
		glColor3f(1.0,1.0,1.0);
		//glTranslatef(antLoc, 0.0, 0.0) ;
		glTranslatef(ant_x_pos, ant_y_pos, ant_z_pos);
		glRotatef(RAD_TO_DEG * antLoc, 0.0, 0.0, -1.0);
		glRotatef(270.0, 0.0, 0.0, 1.0);
		//glScaled(20, 20, 20);

		//Initialize and bind texture
		drawObject(objAnt, 0,0 );
	glPopMatrix();
	
	/*Scatter the pebbles all over*/
	
	for(int i=0; i < 100; i++)
	{
		glPushMatrix();
			glColor3f(0.0f, 0.0f, 0.0f);
			if(i%5 == 0)
				glScalef(8,4,4);
			else if(i%4 == 0)
				glScalef(4,4,4);
			else if(i%3 == 0)
				glScalef(6,4,3);
			else if(i%2 == 0)
				glScalef(2,4,2);

			glTranslatef(pebbleArray[i][0],pebbleArray[i][1],0);
			glCallList(pebble);
			//drawObject(objPebble, 0, 0);
		glPopMatrix();
		glFlush();
	}

/*************\
| Grass |
\*************/
//glPushMatrix();
//glRotatef(grassAngle,0,1,0);
//grassBlade(0.0f, 0.0f, 10.0f, 0.0f, 0.0f);
//glPopMatrix();

//glPushMatrix();
//glTranslatef(1.0,1.0,0.0);
//glRotatef(grassAngle,0,1,0);
//glTranslatef(-1.0,-1.0,0.0);
//grassBlade(1.0f, 1.0f, 12.0f, 10.0f, 0.0f);
//glPopMatrix();
//
//glPushMatrix();
//glTranslatef(-1.0f, 2.0f,0.0);
//glRotatef(grassAngle,0,1,0);
//glTranslatef(1.0f, -2.0f,0.0);
//grassBlade(-1.0f, 2.0f, 8.0f, 10.0f, 10.0f);
//glPopMatrix();

	int grass_choice = 0;
	for(int grass_count = 0; grass_count < 150; grass_count++)
	{
		glPushMatrix();
		glTranslatef(grassArray[grass_count][0], grassArray[grass_count][1],0.0);
		glRotatef(grassAngle,0,1,0);
		glTranslatef(-grassArray[grass_count][0], -grassArray[grass_count][1],0.0);
		//grassBlade(-1.5f, 0.25f, 15.0f, 30.0f, 70.0f);
		if(grass_choice == 4)
			grass_choice = 0;

		printf("the grass_choice is %d\n",grass_choice);
		printf("the grass_count is %d\n",grass_count);

		if(grass_choice == 0)
			grassBlade(grassArray[grass_count][0], grassArray[grass_count][1], 8.0f, 10.0f, 10.0f);
		else if(grass_choice == 1)
			grassBlade(grassArray[grass_count][0], grassArray[grass_count][1], 15.0f, 30.0f, 70.0f);
		else if(grass_choice == 2)
			grassBlade(grassArray[grass_count][0], grassArray[grass_count][1], 12.0f, 10.0f, 0.0f);
		else if(grass_choice == 3)
			grassBlade(grassArray[grass_count][0], grassArray[grass_count][1], 10.0f, 0.0f, 0.0f);

		grass_choice++;
		glPopMatrix();

	}

//glPopMatrix();
	/******************
	*Hills using Nurbs*
	******************

	GLfloat knots[8] = {0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0};
	glPushMatrix();
		glColor3f(0.2f, 0.1f, 0.0f);
		glTranslatef(4.5,-4.0,2.0);
        glRotatef(105.0, 0.0,0.0,1.);
        glScalef (3, 3, 4);
		drawNurbs(theNurb,knots,-1,ctlpoints);        
    glPopMatrix();
*/

	glutSwapBuffers();
	glFlush();
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

    //glDisable(GL_DEPTH_TEST);
    //glDisable(GL_LIGHTING);
    //glDisable(GL_BLEND);

	// Ground

	//glBindTexture(GL_TEXTURE_2D, TexID1);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glColor3f(0.2f, 0.1f, 0.0f);
	
	glBegin( GL_QUADS );
		//glTexCoord2d(  0.0,  0.0);
		glVertex3d(-width,-depth, bottom);
		//glTexCoord2d( 10.0,  0.0);
		glVertex3d( width,-depth, bottom);
		//glTexCoord2d( 10.0,  5.0);
		glVertex3d( width, depth, bottom);
		//glTexCoord2d(  0.0,  5.0);
		glVertex3d(-width, depth, bottom);
	glEnd();
	glFlush();
    glEnable(GL_TEXTURE_2D);

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
	case 'p':
		animate = !animate ;
		if (animate) 
			glutIdleFunc(animation) ;
		else
			glutIdleFunc(NULL) ;
		break ;
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
