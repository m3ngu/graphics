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

float calculateAttenuation(float constatt, float linearatt,float quadatt, float distance)
{
	return (1/(float)(constatt + linearatt*distance + quadatt*distance*distance));
}
LightColor::LightColor(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}
LightColor LightColor::operator + (LightColor c1,LightColor c2)
{
	LightColor c;
	c.r = c1.r + c2.r;
	c.b = c1.b + c2.b;
	c.g = c1.g + c2.g;
	c.a = c1.a + c2.a;
	
	return c;
}

LightColor LightColor::operator * (float f,LightColor c2)
{
	LightColor c;
	c.r = f*c2.r;
	c.b = f*c2.b;
	c.g = f*c2.g;
	c.a = f*c2.a;
	
	return c;
}

LightColor LightColor::operator * (LightColor c1,LightColor c2)
{
	LightColor c;
	c.r = c1.r*c2.r;
	c.b = c1.b*c2.b;
	c.g = c1.g*c2.g;
	c.a = c1.a*c2.a;
	
	return c;
}


LightColor LightColor::operator * (LightColor c2,float f)
{
	LightColor c;
	c.r = f*c2.r;
	c.b = f*c2.b;
	c.g = f*c2.g;
	c.a = f*c2.a;
	
	return c;
}

bool LightColor::operator > (LightColor c1, LightColor c2) {
	if (c1.r > c2.r && c1.g > c2.g && c1.b > c2.b) return true;
	return false;
}