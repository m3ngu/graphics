#ifndef _material_
#define _material_

#include "Light.h"

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
		
		diffuse = Colors(m.diffuse);
		specular = Colors(m.specular);
		shininess = m.shininess;
		emission = Colors(m.emission);
		
	}

};

#endif // _material_