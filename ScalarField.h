#ifndef SCALAR_FIELD
#define SCALAR_FEILD

class ScalarField
{
public:
	float * data;	
	int xRes, yRes;

	ScalarField(void) {}
	~ScalarField(void) {free(data);}

	void init(int xResolution, int yResolution)
	{
		xRes = xResolution;
		yRes = yResolution;
		data = (float*) malloc(sizeof(float) * xRes*yRes);
		for(int i=0; i<xRes*yRes; i++)
			data[i] = 0 ;
	}

	void set(int x, int y, float v)
	{
		data[getIndex(x, y)] = v;
	}

	float operator()(int x, int y)
	{
		return data[getIndex(x, y)];
	}

	int getIndex(int x, int y)
	{
	    return y*xRes + x;
	}
};

#endif