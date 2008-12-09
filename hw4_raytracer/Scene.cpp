/*
 *  parser.cpp
 *
 */
#include "Scene.h"

// This is the main function that defines the scene.  
// For the most part, just read a line and do the appropriate thing.
// The raytracer just needs to replace this with the appropriate function 
// calls for the purpose of parsing.

Scene::Scene() {
	vert = 0;
	vertnorm = 0;
	
	maxverts = 0, maxvertnorms = 0;
	curvert = 0, curvertnorm = 0 ;

	maxdepth = 5 ;
	lightnum = 0 ;

	stck = new StackObject();
}

Scene::~Scene() {
	delete vert;
	delete vertnorm;
}

mat4 Scene::lookAt(Camera *cam) {
	mat4 M;
	mat4 R = mat4_id;
	mat4 T = mat4_id;
	mat3 R3 = mat3_id;
	
	vec3 * u = cam->getU();
	vec3 * v = cam->getV();
	vec3 * w = cam->getW();
	
	vec3 * eye = cam->getEye();
	
	// We put all of these vectors togehter in a 3x3 matrix, where each row is one of the axes
	R3.set_row(0, *u);
	R3.set_row(1, *v);
	R3.set_row(2, *w);
	
	// To get our final rotation matrix, we build a 4x4 rotation matrix from our axes
	// We manually touch the last row so that the homogenous coordinate is 1
	R.set_rot(R3);
	
	vec3 t = *eye - vec3(0,0,0);
	
	//printf("Debug: eye x=%f y=%f z=%f\n", eye->x, eye->y, eye->z);
	//printf("Debug: t x=%f y=%f z=%f\n", t.x, t.y, t.z);
	// And then set the translation using "mat4.set_translation"
	// This translation moves the origin to the location of the camera
	T.set_translation(t);
	
	// Finally, our transformation matrix is the combination to the rotation and translation
	// components
	mult(M,R,T);
	
    return M;
}


void Scene::parsefile (FILE *fp) {
	
	char line[1000], command[1000] ; // Very bad to prefix array size :-)
	Material currMat;
	
	while (!feof(fp)) {
		fgets(line,sizeof(line),fp) ;
		if (feof(fp)) break ;
		if (line[0] == '#') continue ; // Comment lines
		
		int num = sscanf(line, "%s", command) ;
		if (num != 1) continue ; // Blank line etc.
		
		// Now, we simply parse the file by looking at the first line for the 
		// various commands
		
		// *******************  SET IMAGE SIZE *******************
		
		if (!strcmp(command, "size")) {
				
				int num = sscanf(line, "%s %d %d", command, &sizex, &sizey) ;
				
				if (num != 3) {
					fprintf(stderr, "size x y\n") ;
					exit(1) ;
				}
				
				assert(!strcmp(command, "size")) ;
		}
	
		// *******************  CAMERA LOCATION *******************
		
		else if (!strcmp(command, "camera")) {
			float lookfrom[3], lookat[3], up[3], fov ;
			int num = sscanf(line, "%s %f %f %f %f %f %f %f %f %f %f", 
							 command, lookfrom, lookfrom+1, lookfrom+2, 
							 lookat, lookat+1, lookat+2, up, up+1, up+2, &fov) ;
			if (num != 11) {
				fprintf(stderr, "camera from[3] at[3] up[3] fov\n") ;
				exit(1) ;
			}
			
			assert(!strcmp(command,"camera")) ;
			
			cam = new Camera(vec3(lookfrom), vec3(lookat), vec3(up));
			cam->setupFov(fov, &sizex, &sizey);
			
			modelViewMatrix = lookAt(cam);
			/*
			printf("Debug: modelViewMatrix:\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n[%f %f %f %f]\n",
					modelViewMatrix.a00, modelViewMatrix.a01, modelViewMatrix.a02, modelViewMatrix.a03,
					modelViewMatrix.a10, modelViewMatrix.a11, modelViewMatrix.a12, modelViewMatrix.a13,
					modelViewMatrix.a20, modelViewMatrix.a21, modelViewMatrix.a22, modelViewMatrix.a23,
					modelViewMatrix.a30, modelViewMatrix.a31, modelViewMatrix.a32, modelViewMatrix.a33);
			*/
		}
		
		// *************************************************
		// *******************  GEOMETRY *******************
		// *************************************************
		
		// *******************  Sphere *******************
		
		else if (!strcmp(command, "sphere")) {
			float radius; // Syntax is sphere x y z radius 
			double pos[3];
			int num = sscanf(line, "%s %lf %lf %lf %f", command, pos, pos+1, pos+2, &radius);
			if (num != 5) {
				fprintf(stderr, "sphere x y z radius\n");
				exit(1);
			}
			vec3 center = vec3(pos[0],pos[1],pos[2]);
			float rad = radius;
			
			obj* o = new sphere(rad,center);
			o->mat = Material(currMat);
			o->TransformMatrix = stck->getTopofStack();
			objects.push_back(o);
		}
		
		// *******************  Maxverts *******************
		
		else if (!strcmp(command, "maxverts")) {
			int num = sscanf(line, "%s %d", command, &maxverts);
			assert(num == 2);
			assert(maxverts > 0);
			assert(!strcmp(command,"maxverts"));
			assert(vert = new Vertex[maxverts]);
		}
		
		// *******************  Maxvertnorms *******************
		
		else if (!strcmp(command, "maxvertnorms")) {
			int num = sscanf(line, "%s %d", command, &maxvertnorms) ;
			assert(num == 2);
			assert(maxvertnorms > 0);
			assert(!strcmp(command,"maxvertnorms"));
			assert(vertnorm = new VertexNormal[maxvertnorms]);
		}
		
		// *******************  Vertex *******************
		
		else if (!strcmp(command, "vertex")) {
			assert(maxverts);
			assert(curvert < maxverts);
			Vertex v;
			int num = sscanf(line, "%s %lf %lf %lf", command, v.pos, v.pos+1, v.pos+2) ;
			assert(num == 4);
			assert(!strcmp(command,"vertex")) ;
			vert[curvert] = v ; // Add a vertex to the stack
			++curvert ;
		}
		
		// *******************  Vertexnormal *******************
		
		else if (!strcmp(command, "vertexnormal")) {  
			// Add a vertex to the stack with a normal
			assert(maxvertnorms);
			assert(curvertnorm < maxvertnorms);
			VertexNormal vn;
			
			int num = sscanf(line, "%s %lf %lf %lf %lf %lf %lf", 
							 command, vn.pos, vn.pos+1, vn.pos+2, 
							 vn.normal, vn.normal+1, vn.normal+2) ;
			
			assert(num == 7);
			assert(!strcmp(command,"vertexnormal"));
			vertnorm[curvertnorm] = vn ;
			++curvertnorm ;
		}
		
		// *******************  Tri *******************
		
        else if (!strcmp(command, "tri")) {
			// Triangle from 3 vertices
			int pts[3] ; 
			int num = sscanf(line, "%s %d %d %d", command, pts, pts+1, pts+2) ;
			assert(num == 4);
			assert(!strcmp(command,"tri"));
			//int i,j;
			int i;
			for (i = 0 ; i < 3 ; i++) {
				assert(pts[i] >= 0 && pts[i] < maxverts) ;
			}
			
			vec3 A = vec3(vert[pts[0]].pos[0], vert[pts[0]].pos[1], vert[pts[0]].pos[2]);
			vec3 B = vec3(vert[pts[1]].pos[0], vert[pts[1]].pos[1], vert[pts[1]].pos[2]);
			vec3 C = vec3(vert[pts[2]].pos[0], vert[pts[2]].pos[1], vert[pts[2]].pos[2]);
			
			obj* o = new triangle(A,B,C);
			o->mat = Material(currMat);
			o->TransformMatrix = stck->getTopofStack();
			objects.push_back(o);
			
			/*
			double vertex[3][3] ;
			double normal[3] ;

			for (i = 0 ; i < 3 ; i++) 
				for (j = 0 ; j < 3 ; j++)
					vertex[i][j] = vert[pts[i]].pos[j] ;

			// Calculate the face normal 
			double vec1[3], vec2[3], vec3[3] ;
			for (i = 0 ; i < 3 ; i++) {
				vec1[i] = vertex[1][i] - vertex[0][i] ;
				vec2[i] = vertex[2][i] - vertex[0][i] ;
			}
			vec3[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1] ;
			vec3[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2] ;
			vec3[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0] ;
			
			double norm = 0 ;
			for (i = 0 ; i < 3 ; i++) norm += vec3[i] * vec3[i] ;
			norm = sqrt(norm) ;
			if (norm == 0) {normal[0] = 0 ; normal[1] = 0 ; normal[2] = 1.0 ; }
			else {
				for (i = 0 ; i < 3 ; i++) normal[i] = vec3[i] / norm ;
			}
			*/
			
		}
		
		// *******************  Quad *******************
		
        else if (!strcmp(command, "quad")) {
		
			// Quad from 4 vertices
			int pts[4] ; 
			int num = sscanf(line, "%s %d %d %d %d", command, pts, pts+1, pts+2, pts+3) ;
			assert(num == 5);
			assert(!strcmp(command,"quad"));
			
			int i;
			for (i = 0 ; i < 4 ; i++) {
				assert(pts[i] >= 0 && pts[i] < maxverts) ;
			}
			
			vec3 A = vec3(vert[pts[0]].pos[0], vert[pts[0]].pos[1], vert[pts[0]].pos[2]);
			vec3 B = vec3(vert[pts[1]].pos[0], vert[pts[1]].pos[1], vert[pts[1]].pos[2]);
			vec3 C = vec3(vert[pts[2]].pos[0], vert[pts[2]].pos[1], vert[pts[2]].pos[2]);
			vec3 D = vec3(vert[pts[3]].pos[0], vert[pts[3]].pos[1], vert[pts[3]].pos[2]); 
			
			/*
			printf("Debug: A x=%f y=%f z=%f\n", A.x, A.y, A.z);
			printf("Debug: B x=%f y=%f z=%f\n", B.x, B.y, B.z);
			printf("Debug: C x=%f y=%f z=%f\n", C.x, C.y, C.z);
			printf("Debug: D x=%f y=%f z=%f\n", D.x, D.y, D.z);
			*/
			/*
			vec3 A1, B1, C1, D1;
			
			mult_pos(A1, modelViewMatrix, A);
			mult_pos(B1, modelViewMatrix, B);
			mult_pos(C1, modelViewMatrix, C);
			mult_pos(D1, modelViewMatrix, D);
			
			printf("Debug: A1 x=%f y=%f z=%f\n", A1.x, A1.y, A1.z);
			printf("Debug: B1 x=%f y=%f z=%f\n", B1.x, B1.y, B1.z);
			printf("Debug: C1 x=%f y=%f z=%f\n", C1.x, C1.y, C1.z);
			printf("Debug: D1 x=%f y=%f z=%f\n", D1.x, D1.y, D1.z);
			
			myquad = quad(A1,B1,C1,D1); 
			*/
			
			obj* o = new quad(A,B,C,D);
			o->mat = Material(currMat);
			o->TransformMatrix = stck->getTopofStack();
			objects.push_back(o);

			/****
			double vertex[3][3] ;
			double normal[3] ;
			for (i = 0 ; i < 3 ; i++) 
				for (j = 0 ; j < 3 ; j++)
					vertex[i][j] = vert[pts[i]].pos[j] ;
			
			// Calculate the face normal 
			double vec1[3], vec2[3], vec3[3] ;
			for (i = 0 ; i < 3 ; i++) {
				vec1[i] = vertex[1][i] - vertex[0][i] ;
				vec2[i] = vertex[2][i] - vertex[0][i] ;
			}
			
			vec3[0] = vec1[1]*vec2[2] - vec1[2]*vec2[1] ;
			vec3[1] = vec1[2]*vec2[0] - vec1[0]*vec2[2] ;
			vec3[2] = vec1[0]*vec2[1] - vec1[1]*vec2[0] ;
			
			double norm = 0 ;
			for (i = 0 ; i < 3 ; i++) norm += vec3[i] * vec3[i] ;
			norm = sqrt(norm) ;
			if (norm == 0) {normal[0] = 0 ; normal[1] = 0 ; normal[2] = 1.0 ; }
			else {
				for (i = 0 ; i < 3 ; i++) normal[i] = vec3[i] / norm ;
			}
			*****/
			
		}
		
		// *******************  Trinormal *******************
        
		else if (!strcmp(command, "trinormal")) {
			int pts[3] ;
			int num = sscanf(line, "%s %d %d %d", command, pts, pts+1, pts+2) ;
			assert(num == 4);
			assert(!strcmp(command,"trinormal"));
			int i;
			for (i = 0 ; i < 3 ; i++) {
				assert(pts[i] >= 0 && pts[i] < maxvertnorms);
			}
		}
		
		// ********************************************************
		// *******************  TRANSFORMATIONS *******************
		// ********************************************************
		
		// *******************  Translate *******************
		
		else if (!strcmp(command, "translate")) {
			double x,y,z ; // Translate by x y z as in standard OpenGL
			
			int num = sscanf(line, "%s %lf %lf %lf",command, &x, &y, &z) ;
			vec3 translatevec = vec3(x,y,z);
			MatrixTransforms mobj;
			mat4 translate;
			mobj.getTranslate(translatevec, translate);
			stck->pushElement(translate);
			if (num != 4) {
				fprintf(stderr, "translate x y z\n") ;
				exit(1) ;
			}
		}
		
		// *******************  Rotate *******************
		
		else if (!strcmp(command, "rotate")) {
			double ang, x,y,z ; // Rotate by an angle about axis x y z as in standard OpenGL
			
			int num = sscanf(line, "%s %lf %lf %lf %lf",command, &x, &y, &z, &ang) ;
			vec3 rotatevec = vec3(x,y,z);
			MatrixTransforms mobj;
			mat4 rotate;
			mobj.getRotate(rotatevec,ang, rotate);
			stck->pushElement(rotate);
			if (num != 5) {
				fprintf(stderr, "rotate angle x y z\n") ;
				exit(1) ;
			}
		}
		
		// *******************  Scale *******************
		
		else if (!strcmp(command, "scale")) {
			double x,y,z ; // Scale by x y z as in standard OpenGL
			
			int num = sscanf(line, "%s %lf %lf %lf",command, &x, &y, &z) ;
			vec3 scalevec = vec3(x,y,z);
			MatrixTransforms mobj;
			mat4 scale;
			mobj.getScale(scalevec,scale);
			stck->pushElement(scale);

			if (num != 4) {
				fprintf(stderr, "scale x y z\n") ;
				exit(1) ;
			}
		}
		
		// *******************  PushTransform *******************
		
		else if (!strcmp(command, "pushTransform")) {
			// Push the current matrix on the stack as in OpenGL
			int success = stck->push();
			if(success < 0)
				printf("Maximum size of stack is reached\n");
		}
		
		// *******************  PopTransform *******************
		
		else if (!strcmp(command, "popTransform")) {
			stck->pop();
			// Pop the current matrix as in OpenGL
		}
		
		
		// ***********************************************
		// *******************  LIGHTS *******************
		// ***********************************************
		
		// *******************  Directional *******************
				
		else if (!strcmp(command, "directional")) {
			float direction[4], color[4] ; color[3] = 1.0 ; direction[3] = 0.0 ;
			int num = sscanf(line, "%s %f %f %f %f %f %f", command, direction, direction+1, direction+2, color, color+1, color+2) ;
			assert(num == 7) ;
			assert(lightnum >= 0);
			
		}
		
		// *******************  Point *******************
		
		else if (!strcmp(command, "point")) {
			float direction[4], color[4] ; color[3] = 1.0 ; direction[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f %f %f %f", command, direction, direction+1, direction+2, color, color+1, color+2) ;
			assert(num == 7) ;
			assert(lightnum >= 0) ;
		}
		
		// *******************  Attenuation *******************
		
		else if (!strcmp(command, "attenuation")) {
		
			double attenuation[3];
			
			int num = sscanf(line, "%s %lf %lf %lf", command, attenuation, attenuation + 1, attenuation +2) ;
			assert(num == 4) ;
			assert(!strcmp(command, "attenuation")) ;
		}
		// *******************  Ambient *******************
		
		else if (!strcmp(command, "ambient")) {
			float ambient[4] ; ambient[3] = 1.0 ;
			int num = sscanf(line, "%s %f %f %f", command, ambient, ambient+1, ambient+2) ;
			assert(num == 4) ;
			assert(!strcmp(command, "ambient")) ;
		}
		
		// **************************************************
		// *******************  MATERIALS *******************
		// **************************************************
		
		// *******************  Diffuse *******************
				
		else if (!strcmp(command, "diffuse")) {
			float diffuse[3] ;
			int num = sscanf(line, "%s %f %f %f", command, diffuse, diffuse+1, diffuse+2) ;
			assert(num == 4);
			assert (!strcmp(command, "diffuse"));
			
			int i;
			for (i = 0 ; i < 3 ; i++) {
				currMat.diffuse[i] = diffuse[i];
			}
			
			//printf("Debug: diffuse r=%f g=%f b=%f\n",
			//	currMat.diffuse[0],
			//	currMat.diffuse[1],
			//	currMat.diffuse[2]);
		}
		
		// *******************  Specular *******************
		
		else if (!strcmp(command, "specular")) {
			float specular[3] ;
			int num = sscanf(line, "%s %f %f %f", command, specular, specular+1, specular+2) ;
			assert(num == 4) ;
			assert (!strcmp(command, "specular")) ;
			
			int i;
			for (i = 0 ; i < 3 ; i++) {
				currMat.specular[i] = specular[i];
			}
		}
		
		// *******************  Shininess *******************
		
		else if (!strcmp(command, "shininess")) {
			float shininess ;
			int num = sscanf(line, "%s %f", command, &shininess) ;
			assert(num == 2) ; assert (!strcmp(command, "shininess")) ;
			
			currMat.shininess = shininess;
		}
		
		// *******************  Emission *******************
		
		else if (!strcmp(command, "emission")) {
			float emission[3] ;
			int num = sscanf(line, "%s %f %f %f", command, emission, emission+1, emission+2) ;
			assert(num == 4) ; assert (!strcmp(command, "emission")) ;
			
			int i;
			for (i = 0 ; i < 3 ; i++) {
				currMat.emission[i] = emission[i];
			}
		}
		
		// *****************************************************
		// *******************  MISCELLANOUS *******************
		// *****************************************************
		
		// *******************  Maxdepth *******************
		
        else if (!strcmp(command, "maxdepth")) {
			int num = sscanf(line, "%s %d", command, &maxdepth) ;
			assert(num == 2) ;
			assert(!strcmp(command, "maxdepth")) ;
			fprintf(stderr, "Maxdepth set to %d\n", maxdepth) ;
		}
		
		// *******************  Output *******************
		
		else if (!strcmp(command, "output")) {
			int num = sscanf(line, "%s %s", command, &outFileName) ;
			assert(num == 2) ;
			assert(!strcmp(command, "output")) ;
			fprintf(stderr, "Output image file set to: %s\n", outFileName) ;
		}
		
		else {
			fprintf(stderr, "Unimplemented command: %s\n", command) ;
			exit(1) ;
		}
		
	}
}
