#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "SoundObject.h"
#include "Field.h"

class ParticleSystem
{
public:
	SoundObject * particle;
	Field velocityField;
	int xRes, yRes;
	int numParticles;
	SpeakerSetup speakers;
	
	float clearanceRadius;
	float drawRadius;

	void init(int xResolution, int yResolution, int numberOfParticles)
	{
		xRes = xResolution;
		yRes = yResolution;

		speakers.radius = xRes/32.0; // Radius of the speaker setup in pixels
		speakers.quadSetup();	// Setup quadraphonic system

		clearanceRadius = speakers.radius * 1.5;
		drawRadius = 2;

		numParticles = numberOfParticles;
		velocityField.init(xRes, yRes);
		particle = (SoundObject * ) malloc(sizeof(SoundObject) * numParticles);

		for (int i=0; i<numParticles; i++)
		{
			particle[i].init();
			particle[i].initResolution(xRes, yRes);
			particle[i].setInitialPosition( rand() % xRes, rand() % yRes);
		}

		setVelocityField();
	}

	ParticleSystem(void) {}
	~ParticleSystem(void) 
	{
		free(particle);
	}

	void draw(void)
	{
		for (int i=0; i<numParticles; i++)
		{
			particle[i].draw(drawRadius);
		}
	}

	void update(void)
	{
		for (int i=0; i<numParticles; i++)
		{
			particle[i].updatePosition();

			int x1 = particle[i].currentPos[0];
			int y1 = particle[i].currentPos[1];
			particle[i].nextPos = particle[i].currentPos + velocityField(x1,y1);

			// MIRRORING
			// if (particle[i].nextPos[0] >= xRes)
			// 	particle[i].nextPos[0] = particle[i].nextPos[0] - xRes;
			// if (particle[i].nextPos[1] >= yRes)
			// 	particle[i].nextPos[1] = particle[i].nextPos[1] - yRes;
			// if (particle[i].nextPos[0] < 0)
			// 	particle[i].nextPos[0] = particle[i].nextPos[0] + xRes;
			// if (particle[i].nextPos[1] < 0)
			// 	particle[i].nextPos[1] = particle[i].nextPos[1] + yRes;

			// RESPAWNING
			if (particle[i].nextPos[0] >= xRes || particle[i].nextPos[0] < 0 || particle[i].nextPos[1] >= yRes || particle[i].nextPos[1] < 0)
			{
				do
				{
					particle[i].setInitialPosition( rand() % xRes, rand() % yRes);

				} while (particle[i].currentPos.length()<=clearanceRadius);
			}

			particle[i].updateSound(speakers);
		}
	}

	int getIndex(int x, int y)
	{
	    return y*xRes + x;
	}

	ofVec2f velocityFieldFunction_SafeRegion(int x, int y)
	{
		return ofVec2f(x, y);
	}

	ofVec2f velocityFieldFunction(int x, int y)
	{
		// Sample field function - replace x and y with your function
		return ofVec2f(x, y);

		// More samples
		// return ofVec2f(x, y) / pow( x*x + y*y , 1.5);
		// return ofVec2f( x*x ,  y*y );
	}

	void setVelocityField(void)
	{
		for (int x=0; x<xRes; x++)
		for (int y=0; y<yRes; y++)
		{
			ofVec2f location(x, y);
			if(location.length()<=clearanceRadius)
			{
				// TO keep the particles away from the speakers
				ofVec2f velocity = convert(location);
				velocity = velocityFieldFunction_SafeRegion(velocity[0], velocity[0]);
				velocity = convert_back(velocity);
				velocity.normalize();
				velocity *= 500;

				velocityField.set(x, y, velocity);
			}

			else
			{
				// Velocity field
				ofVec2f velocity = convert(location);
				velocity = velocityFieldFunction( velocity[0], velocity[1] );
				velocity = convert_back(velocity);

				velocity.normalize();

				velocityField.set(x, y, velocity);
			}
		}
	}

	ofVec2f convert(ofVec2f &v)
	{
		int x = v[0] - xRes/2; 
		int y = -1*v[1] + yRes/2;
		return ofVec2f(x, y);	
	}

	ofVec2f convert_back(ofVec2f &v)
	{
		int x = v[0] + xRes/ 2 ;
		int y = -1* v[1] + yRes/2 ;
		return ofVec2f(x, y);	
	}

};

#endif