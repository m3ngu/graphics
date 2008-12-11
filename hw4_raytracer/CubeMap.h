#include "Texture.h"
struct cubemap
{
	enum {
		up = 0,
		down = 1,
		right = 2,
		left = 3,
		forward = 4,
		backward = 5
	};
    char *fileup, *filedown, *fileright, *fileleft, *fileforward, *filebackward;
    int sizeX, sizeY;
	Colors *texture; 
	int textureUse;
    //float exposure;
    //bool bExposed;
    //bool bsRGB;
    cubemap() : sizeX(0), sizeY(0), texture(0) , textureUse(0){};
    bool Init();
    //void setExposure(float newExposure) {exposure = newExposure; }
    ~cubemap() { if (texture) delete []texture; }
};

Colors readCubemap(const cubemap & cm, const vec3& origin, const vec3& direction);
