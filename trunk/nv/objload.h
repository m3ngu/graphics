struct objects
{
	unsigned int nverts;
	unsigned int nindices;
	unsigned int *indices;
	float *vertexdata, *normaldata, *texcoords;
	float *tangendata, *binormdata;
};
bool LoadObject(const char *fileName, objects &object, float center[], float scale);

bool LoadObjModel( const char *fileName,
                   unsigned int &nverts, unsigned int &nindices, unsigned int *&indices,
                   float *&vertexdata, float *&normaldata, float *&tangendata, float *&binormdata,
				   float *&texcoords, float center[], float scale );
