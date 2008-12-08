/*
 *  Image.cpp
 */

#include <iostream>
#include "Image.h"

Image::Image() {
	rows = 0;
	columns = 0;
	colors = 0;
}

Image::~Image() {
	if(dataR != NULL) { delete dataR; }
	if(dataG != NULL) { delete dataG; }
	if(dataB != NULL) { delete dataB; }
}

int Image::setSize(int rows, int columns) {
	
	Image::rows=rows;
	Image::columns=columns;
	
	dataR = (uchar *) malloc(rows * columns * sizeof(char));
	if(dataR == NULL) {
		fprintf(stderr, "Insufficient memory for new image\n");
		exit(1); 
	}
	
	dataG = (uchar *) malloc(rows * columns * sizeof(char));
	if(dataG == NULL) {
		fprintf(stderr, "Insufficient memory for new image\n");
		exit(1); 
	}
	
	dataB = (uchar *) malloc(rows * columns * sizeof(char));
	if(dataB == NULL) {
		fprintf(stderr, "Insufficient memory for new image\n");
		exit(1); 
	} 
	
	return 0;
}

int Image::getNCols() {
	return columns;
}

int Image::getNRows() {
	return rows;
}

int Image::setColors(int colors) {
	Image::colors=colors;
	return 0;
}

int Image::getColors() {
	return colors;
}

int Image::setPixel(int i, int j, int colorR, int colorG, int colorB) {
	i=CLAMP(i, 0, rows-1);
	j=CLAMP(j, 0, columns-1);
	dataR[i*(columns)+j]=colorR;
	dataG[i*(columns)+j]=colorG;
	dataB[i*(columns)+j]=colorB;
	return 0;
}

int Image::getPixel(int i, int j, int colorNum) {
	i=CLAMP(i, 0, rows-1);
	j=CLAMP(j, 0, columns-1);
	if (colorNum==1)  return (int) dataR[i*(columns)+j];
	if (colorNum==2)  return (int) dataG[i*(columns)+j];
	if (colorNum==3)  return (int) dataB[i*(columns)+j];
	return -1;
}

int Image::writeImage(const char *fname) {
	FILE *output; 
	int nRows;
	int nCols;
	int colors;
	int i, j;
	
	/* open the file */
	if (!fname || (output=fopen(fname, "wb"))==0)
		return(-1);
	
	nRows=getNRows();
	nCols=getNCols();
	colors=getColors();
	
	/* write the header */
	fprintf(output,"P6\n"); /* magic number */
	fprintf(output,"#\n");  /* empty comment */
	fprintf(output,"%d %d\n%03d\n",nCols,nRows,colors); /* image info */
	
	/* write pixels row by row */
	for(i=0;i<nRows;i++)
    {
		for(j=0;j<nCols;j++)
        {
			int byteR, byteG, byteB;
			byteR=getPixel(i,j, 1);
			if (fputc(byteR,output)==EOF) /* couldn't write */
            {
				fclose(output);
				return -1;
            }      
			byteG=getPixel(i,j, 2);
			if (fputc(byteG,output)==EOF) /* couldn't write */
            {
				fclose(output);
				return -1;
            }      
			byteB=getPixel(i,j, 3);
			if (fputc(byteB,output)==EOF) /* couldn't write */
            {
				fclose(output);
				return -1;
            }      
        }
    }
	
	/* close the file */
	if (fclose(output)==EOF) return -1;
	return 0; /* OK */
}