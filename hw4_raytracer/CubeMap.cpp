#include "CubeMap.h"
#include <cmath>
#include <fstream>

using namespace std;

bool dummyTGAHeader(ifstream &currentfile, int &sizeX, int &sizeY)
{
	char dummy;
	char temp;
	currentfile.get(dummy).get(dummy);
	currentfile.get(temp);                  /* uncompressed RGB */
	if (temp!=2)
		return false;
	currentfile.get(dummy).get(dummy);
	currentfile.get(dummy).get(dummy);
	currentfile.get(dummy);
	currentfile.get(dummy).get(dummy);           /* origin X */
	currentfile.get(dummy).get(dummy);           /* origin Y */
	currentfile.get(temp);
	sizeX = temp;
	currentfile.get(temp);
	sizeX += temp * 256;

	currentfile.get(temp);
	sizeY = temp;
	currentfile.get(temp);
	sizeY += temp * 256;

    currentfile.get(temp);                 /* 24 bit bitmap */
	currentfile.get(dummy);
	return true;
}

bool cubemap::Init()
{
    if (texture)
    {
        return false;
    }
    ifstream currentfile ;
    Colors *currentColor;
    int x,y, dummySizeX, dummySizeY;

    currentfile.open(fileup, ios_base::binary);
    if ((!currentfile)||(!dummyTGAHeader(currentfile, sizeX, sizeY)))
        return false;
    if (sizeX <= 0 || sizeY <= 0)
        return false;
    texture = new Colors[(sizeX * sizeY * 6)];

    int number = cubemap::up * sizeX * sizeY;
    currentColor = 	texture + number;
    for (y = 0; y < sizeY; y++)
    for (x = 0; x < sizeX; x++)
    {
		currentColor->b = currentfile.get() /255.0f;
        currentColor->g = currentfile.get() /255.0f;
        currentColor->r = currentfile.get() /255.0f;
        currentColor++;
    }
    currentfile.close();
	
    for (unsigned i = cubemap::down; i <= cubemap::backward; ++i)
    {
        number = i * sizeX * sizeY;
        currentColor = 	texture + number;
		if(i == cubemap::down)
		{
			//printf("DEBUG: The file is %s\n",filedown);
			currentfile.open(filedown, ios_base::binary);
		}
		else if(i == cubemap::right)
		{
			//printf("DEBUG: The file is %s\n",fileright);
			currentfile.open(fileright, ios_base::binary);
		}
		else if(i == cubemap::left)
		{
			//printf("DEBUG: The file is %s\n",fileleft);
			currentfile.open(fileleft, ios_base::binary);
		}
		else if(i == cubemap::forward)
		{
			//printf("DEBUG: The file is %s\n",fileforward);
			currentfile.open(fileforward, ios_base::binary);
		}
		else
		{
			//printf("DEBUG: The file is %s\n",filebackward);
			currentfile.open(filebackward, ios_base::binary);
		}

        if ((!currentfile)||
            (!dummyTGAHeader(currentfile, dummySizeX, dummySizeY)) ||
            sizeX != dummySizeX || 
            sizeY != dummySizeY)
        {
            // The textures for each face have to be of the same size..
            delete [] texture;
            texture = 0;
            return false;
        }

        for (y = 0; y < sizeY; y++)
        for (x = 0; x < sizeX; x++)
        {
            currentColor->b = currentfile.get() /255.0f;
            currentColor->g = currentfile.get() /255.0f;
            currentColor->r = currentfile.get() /255.0f;
            currentColor++;
        }
        currentfile.close();
    }

    return true;
}

Colors readCubemap(const cubemap & cm, const vec3& origin, const vec3& dir)
{
    Colors * currentColor ;
    Colors outputColor = Colors(0.0f,0.0f,0.0f,1.0f);
    if(!cm.texture)
    {
        return outputColor;
    }
    if ((fabsf(dir.x) >= fabsf(dir.y)) && (fabsf(dir.x) >= fabsf(dir.z)))
    {
        if (dir.x > 0.0f)
        {
            currentColor = cm.texture + cubemap::right * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                1.0f - (dir.z / dir.x+ 1.0f) * 0.5f,  
                (dir.y / dir.x+ 1.0f) * 0.5f, cm.sizeX, cm.sizeY);
        }
        else if (dir.x < 0.0f)
        {
            currentColor = cm.texture + cubemap::left * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                1.0f - (dir.z / dir.x+ 1.0f) * 0.5f,
                1.0f - (dir.y / dir.x + 1.0f) * 0.5f,  
                cm.sizeX, cm.sizeY);
        }
    }
    else if ((fabsf(dir.y) >= fabsf(dir.x)) && (fabsf(dir.y) >= fabsf(dir.z)))
    {
        if (dir.y > 0.0f)
        {
            currentColor = cm.texture + cubemap::up * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                (dir.x / dir.y + 1.0f) * 0.5f,
                1.0f - (dir.z/ dir.y + 1.0f) * 0.5f, cm.sizeX, cm.sizeY);
        }
        else if (dir.y < 0.0f)
        {
            currentColor = cm.texture + cubemap::down * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                1.0f - (dir.x / dir.y + 1.0f) * 0.5f,  
                (dir.z/dir.y + 1.0f) * 0.5f, cm.sizeX, cm.sizeY);
        }
    }
    else if ((fabsf(dir.z) >= fabsf(dir.x)) && (fabsf(dir.z) >= fabsf(dir.y)))
    {
        if (dir.z > 0.0f)
        {
            currentColor = cm.texture + cubemap::forward * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                (dir.x / dir.z + 1.0f) * 0.5f,  
                (dir.y/ dir.z + 1.0f) * 0.5f, cm.sizeX, cm.sizeY);
        }
        else if (dir.z < 0.0f)
        {
            currentColor = cm.texture + cubemap::backward * cm.sizeX * cm.sizeY;
            outputColor = readTexture(currentColor,  
                (dir.x / dir.z + 1.0f) * 0.5f,  
                1.0f - (dir.y /dir.z +1) * 0.5f, cm.sizeX, cm.sizeY);
        }
    }
    //if (cm.bsRGB)
    //{
    //   // We make sure the data that was in sRGB storage mode is brought back to a 
    //   // linear format. We don't need the full accuracy of the sRGBEncode function
    //   // so a powf should be sufficient enough.
    //   outputColor.blue   = powf(outputColor.blue, 2.2f);
    //   outputColor.red    = powf(outputColor.red, 2.2f);
    //   outputColor.green  = powf(outputColor.green, 2.2f);
    //}

    //if (cm.bExposed)
    //{
    //    // The LDR (low dynamic range) images were supposedly already
    //    // exposed, but we need to make the inverse transformation
    //    // so that we can expose them a second time.
    //    outputColor.blue  = -logf(1.001f - outputColor.blue);
    //    outputColor.red   = -logf(1.001f - outputColor.red);
    //    outputColor.green = -logf(1.001f - outputColor.green);
    //}

    //outputColor.blue  /= cm.exposure;
    //outputColor.red   /= cm.exposure;
    //outputColor.green /= cm.exposure;

    return outputColor;
}
