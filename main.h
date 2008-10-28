/*
 * main.h
 *
 *  Created on: Oct 28, 2008
 *      Author: msukan
 */

#include "nv/objload.h"

int main(int argc, char* argv[]);
void init();
void display();

void drawObject(objects obj, GLuint TexID, int texture);
GLuint LoadTexture(char *TexName);
void skybox(float width, float depth, float height);

void keyboard(unsigned char key, int x, int y);
void specialKey(int key, int x, int y);
void reshape(int width, int height);
void printHelp();
