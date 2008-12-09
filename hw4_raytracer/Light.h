#include "nv/nv_math.h"
#include<vector>

extern double ATTENUATION_DEFAULT[3];
extern int lightnum;
extern bool linearatt;
extern bool quadatt;
extern bool constatt;
extern const float DIRLIGHTALPHA;
extern const float POINTLIGHTALPHA;
extern const float DEFAULTCOLORALPHA;
extern const float EPSILON;
extern const int MAX_DEPTH;
extern const float DEFAULTATTENUATION[3];

using namespace std;

class Colors
{
public:
	float r, g, b, a;
	Colors();
	Colors(float red, float green, float blue, float alpha);
	Colors operator+(Colors& c);
	Colors operator*(float f);
	Colors operator*(Colors& c);
	bool operator>(Colors& c);
	virtual ~Colors(){};	
};

class Lights
{
public:
	Lights(int directionFlag, vec3 dirorpos, Colors lgtColor);
	vec3 directionorpos;
	Colors lightColor;
	float attenuation;
	int dirFlag;
	void calculateAttenuation(float constatt, float linearatt,float quadatt, float distance);
	virtual ~Lights(){};
};

std::vector<Lights> LightList;


