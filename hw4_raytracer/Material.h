#ifndef _material_
#define _material_

struct Material {
	float diffuse[3];
	float specular[3];
	float shininess;
	float emission[3];
	
	// Default Material
	Material() {
		diffuse[0] = 0.0f;
		diffuse[1] = 0.0f;
		diffuse[2] = 0.0f;
		
		specular[0] = 0.0f;
		specular[1] = 0.0f;
		specular[2] = 0.0f;	
		
		shininess = 0.0f;
		
		emission[0] = 0.0f;
		emission[1] = 0.0f;
		emission[2] = 0.0f;	
	}
	
	// Copy material
	Material(const Material& m) {
		diffuse[0] = m.diffuse[0];
		diffuse[1] = m.diffuse[1];
		diffuse[2] = m.diffuse[2];
		
		specular[0] = m.specular[0];
		specular[1] = m.specular[1];
		specular[2] = m.specular[2];	
		
		shininess = m.shininess;
		
		emission[0] = m.emission[0];
		emission[1] = m.emission[1];
		emission[2] = m.emission[2];	
	}

};

#endif // _material_