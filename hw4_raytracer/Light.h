
#ifndef _light_
#define _light_

#include "nv/nv_math.h"
#include <vector>

struct Colors
{
	float r, g, b, a;
	Colors();
	Colors(float red, float green, float blue, float alpha);
	Colors(const Colors& c);
	virtual ~Colors(){};
	
	bool operator==(const Colors & c) const
    {
        return (c.r == r && c.g == g && c.b == b) ? true : false;
    }
};

inline Colors::Colors()
{
	r = 0.0f;
	g = 0.0f;
	b = 0.0f;
	a = 0.0f;
}

inline Colors::Colors(float red, float green, float blue, float alpha)
{
	r = red;
	g = green;
	b = blue;
	a = alpha;
}

inline Colors::Colors(const Colors& c)
{
	r = c.r;
	g = c.g;
	b = c.b;
	a = c.a;
}

inline const Colors operator*(const Colors& c1, const Colors& c2)
{
	return Colors(c1.r * c2.r,
				  c1.g * c2.g,
				  c1.b * c2.b,
				  c1.a * c2.a);
}

inline const Colors operator+(const Colors& c1, const Colors& c2)
{
	return Colors(c1.r + c2.r,
				  c1.g + c2.g,
				  c1.b + c2.b,
				  c1.a + c2.a);
}

inline const Colors operator*(const Colors& c1, const float f)
{
	return Colors(f * c1.r,
				  f * c1.g,
				  f * c1.b,
				  f * c1.a);
}

inline bool operator>(const Colors& c1, const Colors& c2)
{
	return (c1.r > c2.r
	     && c1.g > c2.g
		 && c1.b > c2.b);
}

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
	
	// Default Attenuation
	Attenuation() {		
		constant  = 1.0f;
		linear    = 0.0f;
		quadratic = 0.0f;
	}
	
	// Copy material
	Attenuation(const Attenuation& a) {
		constant  = a.constant;
		linear    = a.linear;
		quadratic = a.quadratic;
	}

};

class Lights
{
public:
	Lights(int directionFlag, vec3 dirorpos, Colors lgtColor);
	vec3 directionorpos;
	Colors lightColor;
	Attenuation attenuation;
	int dirFlag;
	float calculateAttenuation(float distance,Attenuation attn);
	virtual ~Lights(){};
};

inline Lights::Lights(int directionFlag, vec3 dirorpos, Colors lgtColor) {
	dirFlag = directionFlag;
	attenuation = Attenuation();
	lightColor = lgtColor;
	directionorpos = dirorpos;
	if (dirFlag) {
		directionorpos.normalize();
	}
}

inline float Lights::calculateAttenuation(float distance, Attenuation attn)
{
	attenuation = attn;
	return (1/(float)(attenuation.constant
	                + attenuation.linear * distance
					+ attenuation.quadratic * distance * distance));
}

#endif // _light_
