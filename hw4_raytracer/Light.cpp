#include<stdio.h>
#include "Light.h"

double ATTENUATION_DEFAULT[3] = {1.0, 0.0, 0.0};
int lightnum;
bool linearatt = false;
bool quadatt = false;
bool constatt = false;
const float DIRLIGHTALPHA = 1.0;
const float POINTLIGHTALPHA = 1.0;
const float DEFAULTCOLORALPHA = 1.0;
const float EPSILON = 0.0025;
const int MAX_DEPTH = 16;
const float DEFAULTATTENUATION[3] = {1.0,0,0};

Colors::Colors()
{
	r = 0.0;
	g = 0.0;
	b = 0.0;
	a = 0.0;
}
Colors::Colors(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}
Colors Colors::operator + (Colors c1,Colors c2)
{
	Colors c;
	c.r = c1.r + c2.r;
	c.b = c1.b + c2.b;
	c.g = c1.g + c2.g;
	c.a = c1.a + c2.a;
	
	return c;
}

Colors Colors::operator * (float f,Colors c2)
{
	Colors c;
	c.r = f*c2.r;
	c.b = f*c2.b;
	c.g = f*c2.g;
	c.a = f*c2.a;
	
	return c;
}

Colors Colors::operator * (Colors c1,Colors c2)
{
	Colors c;
	c.r = c1.r*c2.r;
	c.b = c1.b*c2.b;
	c.g = c1.g*c2.g;
	c.a = c1.a*c2.a;
	
	return c;
}


Colors Colors::operator * (Colors c2,float f)
{
	Colors c;
	c.r = f*c2.r;
	c.b = f*c2.b;
	c.g = f*c2.g;
	c.a = f*c2.a;
	
	return c;
}

bool Colors::operator > (Colors c1, Colors c2) {
	if (c1.r > c2.r && c1.g > c2.g && c1.b > c2.b) return true;
	return false;
}

Lights::Lights(int directionFlag, vec3 dirorpos, Colors lgtColor) {
	dirFlag = directionFlag;
	attenuationcoeffs = DEFAULTATTENUATION;
	attenuation = 1.0;
	lightColor = lgtcolor;
	directionorpos = dirorpos;
}

float Lights::calculateAttenuation(float constatt, float linearatt,float quadatt, float distance)
{
	return (1/(float)(constatt + linearatt*distance + quadatt*distance*distance));
}