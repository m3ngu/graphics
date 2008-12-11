#ifndef _material_
#define _material_

#include "Light.h"
#include "CubeMap.h"

const Colors BLACK = Colors(1.0f,0.0f,0.0f,0.0f);

struct Material {
	Colors diffuse;
	Colors specular;
	float shininess;
	Colors emission;
	float reflect;
	float translucency;
	float refract;
	int isCubeMap;
	char *fileup, *filedown, *fileright, *fileleft, *fileforward, *filebackward;
	
	// Default Material
	Material() {
		diffuse = BLACK;
		specular = BLACK;
		shininess = 0.0f;
		emission = BLACK;
		reflect = 0.0f;
		translucency = 0.0f;
		refract = 0.0f;
	}
	
	// Copy material
	Material(const Material& m) {
		
		diffuse = Colors(m.diffuse);
		specular = Colors(m.specular);
		shininess = m.shininess;
		emission = Colors(m.emission);
		reflect = m.reflect;
		translucency = m.translucency;
		refract = m.refract;
	}

};

#endif // _material_