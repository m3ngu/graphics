#include "Texture.h"
#include <cmath>
#include <fstream>

using namespace std;

Colors readTexture(const Colors* tab, float u, float v, int sizeU, int sizeV)
{
    u = fabsf(u);
    v = fabsf(v);
    int umin = int(sizeU * u);
    int vmin = int(sizeV * v);
    int umax = int(sizeU * u) + 1;
    int vmax = int(sizeV * v) + 1;
    float ucoef = fabsf(sizeU * u - umin);
    float vcoef = fabsf(sizeV * v - vmin);
    
    // The texture is being addressed on [0,1]
    // There should be an addressing type in order to 
    // determine how we should access texels when
    // the coordinates are beyond those boundaries.

    // Clamping is our current default and the only
    // implemented addressing type for now.
    // Clamping is done by bringing anything below zero
    // to the coordinate zero
    // and everything beyond one, to one.
    umin = min(max(umin, 0), sizeU - 1);
    umax = min(max(umax, 0), sizeU - 1);
    vmin = min(max(vmin, 0), sizeV - 1);
    vmax = min(max(vmax, 0), sizeV - 1);

    // What follows is a bilinear interpolation
    // along two coordinates u and v.

    Colors output = 
        (tab[umin  + sizeU * vmin] * (1.0f - ucoef)
        + tab[umax + sizeU * vmin] * ucoef)*(1.0f - vcoef)
        + (tab[umin  + sizeU * vmax] * (1.0f - ucoef)
        +  tab[umax + sizeU * vmax] * ucoef ) * vcoef;
    return output;
}
