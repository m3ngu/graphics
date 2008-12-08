#include <math.h>

#define CLAMP(A,L,H) ((A)<=(L) ? (L) : (A)<=(H) ? (A) : (H))

/** Simple typedef for an unsigned character (8-bit values from 0-255) */
typedef unsigned char uchar;


/** A color image with upto 3 channels */
class Image {
	int rows;
	int columns;
	int colors;
	
	uchar * dataR;
	uchar * dataG;
	uchar * dataB;

  public:
	Image ();
	~Image ();
	
	/** Sets the size of the given color image.
	 *  The size is a given 
	 *  height (# of rows) and width (# of columns).
	 *  returns 0 if OK or -1 if fails.
	 */
	int setSize(int rows, int columns);

	/** Returns the number of columns in the given color image.  */
	int getNCols();

	/** Returns the number of rows in the given color image. */
	int getNRows();

	/** Sets the number of gray-level colors in the image (for all channels, not counting 0).
	 *  Returns 0 if OK or -1 if fails.
	 */
	int setColors(int colors);

	/** Returns the number of gray-levels in the color image. */
	int getColors();

	/** Sets the pixel in the color image at row i and column j to the given color. */
	int setPixel(int i, int j, int colorR, int colorG, int colorB);

	/** Returns the color of the pixel in the color image at row i and column j and with color colorNum. */	
	int getPixel(int i, int j, int colorNum);

	/** Writes the color image to fname.
	 *  Returns 0 if OK or -1 if something goes wrong.
	 */
	int writeImage(const char *fname);
  
};