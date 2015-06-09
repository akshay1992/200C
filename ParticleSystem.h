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

};

#endif