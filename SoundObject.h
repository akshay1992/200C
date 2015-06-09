#ifndef SOUND_OBJECT
#define SOUND_OBJECT

#include "Particle.h"
#include "SpeakerSetup.h"

class SoundObject : public Particle
{
public:
	float * gain;
	int nChannels;
	ofMutex gainMutex;
	double phase;
	double phase_increment;
	int xRes, yRes;

	SoundObject(void);

	void init(int channels = 4)
	{
		nChannels = channels;
		gain = (float* ) calloc(nChannels, sizeof(float));
		phase = 0;
		phase_increment = 0.08;
		// phase_increment = (float) rand() / RAND_MAX / 100 ;
	}

	void initResolution(int xResolution, int yResolution)
	{
		xRes = xResolution;
		yRes = yResolution;
	}

	void updateSound(SpeakerSetup &speakers)
	{
		
		ofVec2f d(currentPos[0] - xRes*0.5, - currentPos[1] + yRes*0.5);
		float distGain = compute_dist_gain(d, speakers.radius);

		// *********************** GAIN MUTEX START ***********************
		// gainMutex.lock();
		for(int i=0; i<nChannels; i++)
		{
			gain[i] = compute_azimuth_gain(d, speakers.channel(i));
		}

		float norm = magnitude4f(gain[0], gain[1], gain[2], gain[3]);
		for(int i=0; i<nChannels; i++)
		{
			// Normalize gains
			gain[i] /= norm;
			// Distance-based gain
			gain[i] *= distGain;
		}
		// gainMutex.unlock();
		// ************************* GAIN MUTEX END ************************

		// cout << gain[0] << " " << 
		// 	    gain[1] << " " <<
		//  		gain[2] << " " <<
		//  		gain[3] << " " << endl;
		// cout << "    Power: "<<  magnitude4f(gain[0], gain[1], gain[2], gain[3]) << endl;
	}

	float compute_dist_gain(ofVec2f &object, float speaker_radius)
	{
		float relative_distance = object.length()/speaker_radius;
		if (relative_distance<1)
			relative_distance = 1;
		return 1.0/relative_distance;
	}

	float magnitude4f(float a, float b, float c, float d)
	{
		return sqrt(a*a+b*b+c*c+d*d);
	}

	float compute_azimuth_gain(ofVec2f &object, ofVec2f &speaker)
	{
		float angle = abs(object.angle(speaker));
		float angleRad = abs(object.angleRad(speaker));

		// Linear trend (with 'active pair')
		// int active = angle>90 ? 0 : 1;
		// return active*( 1.0 - ( angle / 90 ) )*distGain;

		// Linear trend (without 'active pair')
		return ( 1.0 - ( angle / 180 ) ) ;
	}

};

#endif
