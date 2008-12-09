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

class Color
{
public:
	float r, g, b, a;
	LightColor(float red, float green, float blue, float alpha);
	LightColor operator + (LightColor c1,LightColor c2);
	LightColor operator * (float f,LightColor c2);
	LightColor operator * (LightColor c1,LightColor c2);
	LightColor operator * (LightColor c2,float f);
	bool operator > (LightColor c1, LightColor c2);
	virtual ~LightColor(){};	
}
struct DirectionalLight
{
	vec3 dir;
	LightColor color;
};

struct PointLight
{
	vec3 pos;
	LightColor color;
	float attenuationcoeffs[3];
	float attenuation;
};

std::vector<PointLight> pointLightList;
std::vector<DirectionalLight> dirLightList;

float calculateAttenuation(float constatt, float linearatt,float quadatt, float distance);
