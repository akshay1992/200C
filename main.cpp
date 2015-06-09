#include "ofMain.h"
#include "ofApp.h"

extern int xRes, yRes;

//========================================================================
int main( ){

	ofSetupOpenGL(xRes,yRes, OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp( new ofApp());

}
