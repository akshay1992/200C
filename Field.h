#ifndef FIELD
#define FEILD

class Field
{
public:
	ofVec2f * data;	
	int xRes, yRes;

	Field(void) {}

	void init(int xResolution, int yResolution)
	{
		xRes = xResolution;
		yRes = yResolution;
		data = (ofVec2f*) malloc(sizeof(ofVec2f) * xRes*yRes);
		for(int i=0; i<xRes*yRes; i++)
			data[i].set(0,0);
	}

	void set(int x, int y, ofVec2f &v)
	{
		data[getIndex(x, y)] = v;
	}

	ofVec2f operator()(int x, int y)
	{
		return data[getIndex(x, y)];
	}

	int getIndex(int x, int y)
	{
	    return y*xRes + x;
	}
};

#endif