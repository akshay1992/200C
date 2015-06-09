#ifndef SPEAKER_SETUP
#define SPEAKER_SETUP

/*
 *     QUAD SETUP
 *
 * Channel 1 - Front Right
 * Channel 2 - Front Left
 * Channel 3 - Back Left
 * Channel 4 - Back Right
 */


class SpeakerSetup
{
public:
	float radius;
	int nChannels;
	ofVec2f* position;
	ofVec2f error;

	SpeakerSetup(void)
	{
		error = ofVec2f(0,0);
	}

	void quadSetup(void)
	{
		if(radius == 0)
		{
			cout << "Speaker Setup Failed. Radius not set" << endl;
			exit(-1);
		}

		nChannels = 4;
		radius = radius;
		position = (ofVec2f*) calloc(4, sizeof(ofVec2f));
		position[0] = ofVec2f(1,1) / sqrt(2) * radius;
		position[1] = ofVec2f(-1,1) / sqrt(2) * radius;
		position[2] = ofVec2f(-1,-1) / sqrt(2) * radius;
		position[3] = ofVec2f(1,-1)/ sqrt(2) * radius;

	}

	ofVec2f& channelFR(void) { return channel(0);}
	ofVec2f& channelFL(void) { return channel(1);}
	ofVec2f& channelRL(void) { return channel(2);}
	ofVec2f& channelRR(void) { return channel(3);}

	ofVec2f& channel(int channelIndex)
	{
		if (channelIndex >= nChannels || channelIndex < 0 )
		{
			cout << "Warning! Invalid channel index: " << channelIndex;
			return error;
		}
		else
		{
			return position[channelIndex];
		}
	}
};

#endif
