#ifndef WALKER_H
#define WAKLER_H

class Walker
{
public:
	ofVec2f currentPos;
	ofVec2f prevPos;
	int stepSize;
	int xRes, yRes;
	int offTheGrid;

	Walker(void) {}

	Walker(int xResolution, int yResolution) 
	{
		init( xResolution, yResolution ); 
	}

	void init(int xResolution, int yResolution)
	{
		xRes = xResolution;
		yRes = yResolution;

		setStartingPosition();
	}

	void reinit(void)
	{
		offTheGrid = 0;

		setStartingPosition();
	}

	void respawn(void)
	{
		offTheGrid = 0;
		// setStartingPosition();
		spawn_anywhere();
	}

	void walk()
	{
		prevPos = currentPos;
		ofVec2f newPos;

		int side = rand() % 4;
		switch(side)
		{
			case 0:	
					newPos = currentPos + stepSize*ofVec2f(0,-1); // Up
					break;
			case 1:
					newPos = currentPos + stepSize*ofVec2f(0,1); // Down
					break;
			case 2:
					newPos = currentPos + stepSize*ofVec2f(-1,0); // Left
					break;
			case 3:
					newPos = currentPos + stepSize*ofVec2f(1,0); // Right
					break;
		}

		if ( newPos[0] >= xRes || newPos[0] <0 )
		{
			offTheGrid = 1;
		}
		if ( newPos[1] >= yRes || newPos[1] <0 )
		{
			offTheGrid = 1;
		}
		else 
		{
			float N = 1.5;
			ofVec2f drawPos = ((currentPos * (N - 1)) + newPos) / N; 

			currentPos = drawPos;
			offTheGrid = 0;
		}
	}

	bool isOffTheGrid(void)
	{
		return offTheGrid;
	}

	void setStartingPosition(void)
	{
		int side = rand() % 4;
		switch(side)
		{
			case 0:	
					currentPos.set( rand() % xRes , 0 ); // top
					break;
			case 1:
					currentPos.set( rand() % xRes , yRes-1 ); // bottom
					break;
			case 2:
					currentPos.set( 0 , rand() % yRes ); // Left
					break;
			case 3:
					currentPos.set( xRes , rand() % yRes ); // Right
					break;
		}
	}

	void spawn_anywhere()
	{
		currentPos.set( rand() % xRes, rand() % yRes);
	}
};


#endif
