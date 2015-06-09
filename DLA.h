#ifndef DLA_H
#define DLA_H

#include "ofImage.h"
#include "Walker.h"
#define loop(xRes, yRes) for(int x=0; x<xRes; x++)	   \
							for(int y=0; y<yRes; y++) 

class DLA : public Walker
{
public:
	ofImage current_frame;
	vector<ofVec2f> aggregate;
	ofMutex mutex;

	DLA(void) {}
	DLA(int xResolution, int yResolution) { init(xResolution, yResolution); }

	void init(int xResolution, int yResolution)
	{
		xRes = xResolution;
		yRes = yResolution;

		current_frame.allocate(xRes, yRes, OF_IMAGE_GRAYSCALE);

		ofVec2f initialAggregate(xRes/2, yRes/2);
		aggregate.push_back(initialAggregate);

		stepSize = 20;
	}

	void update(void)
	{
		walk();
		if ( isOffTheGrid() ) { reinit(); }
		addToAggregate();	

		draw_walker();
		draw_aggregate();
		current_frame.update();

	}

	void draw(int windowY,int windowX)
	{
		current_frame.draw(0,0, windowX, windowY);
	}


	int getIndex(int x, int y)
	{
	    return y*xRes + x;
	}

	int getIndex(ofVec2f &v)
	{
	    return v[1]*xRes + v[0];
	}

	void draw_walker(void)
	{
		current_frame.getPixelsRef()[getIndex(prevPos[0], prevPos[1])] = 0;
		current_frame.getPixelsRef()[getIndex(currentPos[0], currentPos[1])] = 255;
	}

	void draw_aggregate(void)
	{
		for( unsigned int i=0; i<aggregate.size(); i++)
		{
			current_frame.getPixelsRef()[getIndex((aggregate[i])[0], (aggregate[i])[1])] = 255;
		}
	}

	void addToAggregate()
	{
		int x = currentPos[0];
		int y = currentPos[1];

		int add = 0;

		for(unsigned int i=0; i<aggregate.size(); i++)
		{
			if (ofVec2f(x+1, y) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x-1, y) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x, y+1) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x, y-1) == aggregate[i])
			{
				add = 1;
				break;
			}

			if (ofVec2f(x+1, y+1) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x-1, y-1) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x-1, y+1) == aggregate[i])
			{
				add = 1;
				break;
			}
			if (ofVec2f(x+1, y-1) == aggregate[i])
			{
				add = 1;
				break;
			}

			if (add==1)
			{
				aggregate.push_back(ofVec2f(x, y));
			}
			
		}
	}
};



#endif