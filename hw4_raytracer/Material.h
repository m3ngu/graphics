#include "Light.h"
#ifndef _material_
#define _material_

struct Material {
	Colors diffuse;
	Colors specular;
	float shininess;
	Colors emission;
	
	// Default Material
	Material() {		
		shininess = 0.0f;		
	}
	
	// Copy material
	Material(const Material& m) {
		diffuse = m.diffuse;
		specular = m.specular;
		shininess = m.shininess;
		emission = m.emission;
	}

};

#endif // _material_