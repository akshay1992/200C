#ifndef PARTICLE
#define PARTICLE

class Particle
{
public:
	ofVec2f nextPos;
	ofVec2f currentPos;
	ofVec2f	prevPos;

	void update()
	{
		if( nextPos==currentPos)
		{
			return;
		}
		else
		{
			prevPos = currentPos;
			currentPos = nextPos;
		}

	}

	void draw(float radius)
	{
		ofCircle(currentPos[0], currentPos[1], radius);
	}

	float radialVelocity(void)
	{
		return currentPos.length() - prevPos.length();
	}

	void setPosition(int x, int y)
	{
		currentPos.set(x, y);
	}

	void setInitialPosition(int x, int y)
	{
		nextPos.set(x, y);
		prevPos.set(x, y);
		currentPos.set(x, y);
	}

};

#endif