#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "Particle.h"
#include "Field.h"

class ParticleSystem
{
public:
	Particle * particle;
	Field velocityField;
	int xRes, yRes;
	int numParticles;

	float clearanceRadius;
	float drawRadius;

	void init(int xResolution, int yResolution, int numberOfParticles)
	{
		numParticles = numberOfParticles;
		xRes = xResolution;
		yRes = yResolution;
		velocityField.init(xRes, yRes);
		particle = (Particle * ) malloc(sizeof(Particle) * numParticles);

		for (int i=0; i<numParticles; i++)
		{
			particle[i].setInitialPosition( rand() % xRes, rand() % yRes);
		}
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

			particle[i].update();
		}
	}

	int getIndex(int x, int y)
	{
	    return y*xRes + x;
	}

	virtual ofVec2f velocityFieldFunction(int x, int y)
	{
		// Sample field function - replace x and y with your function
		return ofVec2f(x, y);

		// More samples
		return ofVec2f(x, y) / pow( x*x + y*y , 1.5);

		return ofVec2f( x*x ,  y*y );
	}

	void setVelocityField()
	{
		for (int x=0; x<xRes; x++)
		for (int y=0; y<yRes; y++)
		{
			ofVec2f location(x, y);
			if(location.length()<=1.5*clearanceRadius)
			{
				// TO keep the particles away from the speakers
				ofVec2f velocity = convert(location);
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
		int y = -v[1] + yRes/2;
		return ofVec2f(x, y);	
	}

	ofVec2f convert_back(ofVec2f &v)
	{
		int x = v[0] + xRes/ 2 ;
		int y = - v[1] + yRes/2 ;
		return ofVec2f(x, y);	
	}

};

#endif