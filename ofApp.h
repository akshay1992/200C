#pragma once

#include "ofMain.h"

#include "SpeakerSetup.h"
#include "DLA.h"

class ofApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void audioOut( float * output, int bufferSize, int nChannels );
  		double phase;

  		float gainFL, gainFR, gainRL, gainRR;
  		ofMutex gainMutex;

		int mouseX;
		int mouseY;
		ofMutex mouseMutex;
		SpeakerSetup speakers;

		DLA dla;
};
