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
//const float EPSILON = 0.0025;
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
Colors Colors::operator+(Colors& c)
{
	Colors cret;
	cret.r = a + c.a;
	cret.b = b + c.b;
	cret.g = g + c.g;
	cret.a = a + c.a;
	
	return cret;
}

Colors Colors::operator*(float f)
{
	Colors cret;
	cret.r = f * r;
	cret.b = f * b;
	cret.g = f * g;
	cret.a = f * a;
	
	return cret;
}

Colors Colors::operator*(Colors& c)
{
	Colors cret;
	cret.r = r * c.r;
	cret.b = b * c.b;
	cret.g = g * c.g;
	cret.a = a * c.a;
	
	return cret;
}

bool Colors::operator > (Colors& c) {
	if (r > c.r && g > c.g && b > c.b) return true;
	return false;
}

Lights::Lights(int directionFlag, vec3 dirorpos, Colors lgtColor) {
	dirFlag = directionFlag;
	attenuation = 1.0;
	lightColor = lgtColor;
	directionorpos = dirorpos;
}
void Lights::calculateAttenuation(float constatt, float linearatt,float quadatt, float distance)
{
	attenuation =  (1/(float)(constatt + linearatt*distance + quadatt*distance*distance));
}