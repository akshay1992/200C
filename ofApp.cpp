#include "ofApp.h"
#include <assert.h>

int xRes = 500;
int yRes = 500;

int windowX = xRes;
int windowY = yRes;

int animate = 0;

//--------------------------------------------------------------

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

// 
void ofApp::setup(){
	dla.init(xRes, yRes);


	speakers.radius = xRes/32.0; // Radius of the speaker setup in pixels
	speakers.quadSetup();	// Setup quadraphonic system
	phase = 0;
	ofSoundStreamSetup(4, 0); // 4 output channels, 0 input channels
}

//--------------------------------------------------------------
void ofApp::update(){

	int xPos, yPos;

	// *** MOUSE MUTEX ***
	mouseMutex.lock();
	xPos = mouseX;
	yPos = mouseY;
	mouseMutex.unlock();
	// *** MOUSE MUTEX ***
	
	xPos = xPos - xRes*0.5;
	yPos = - yPos + yRes*0.5;

	ofVec2f d(xPos, yPos);
	float distGain = compute_dist_gain(d, speakers.radius);

    // *** GAIN MUTEX START ***
	gainMutex.lock();
	gainFR = compute_azimuth_gain(d, speakers.channelFR());
	gainFL = compute_azimuth_gain(d, speakers.channelFL());
	gainRL = compute_azimuth_gain(d, speakers.channelRL());
	gainRR = compute_azimuth_gain(d, speakers.channelRR());

	// Normalize gains
	float norm = magnitude4f(gainFR, gainFL, gainRL, gainRR);
	gainFR /= norm;
	gainFL /= norm;
	gainRL /= norm;
	gainRR /= norm;

	// Distance-based gain
	gainFR *= distGain; 
	gainFL *= distGain; 
	gainRL *= distGain; 
	gainRR *= distGain; 	

	gainMutex.unlock();
	// *** GAIN MUTEX END ***

	cout << gainFR << " " << 
		    gainFL << " " <<
	 		gainRL << " " <<
	 		gainRR << " " << endl;
	cout << "    Power: "<<  magnitude4f(gainFR, gainFL, gainRL, gainRR) << endl;


	if(animate)
	{
		dla.update();
	}


}
//--------------------------------------------------------------
void ofApp::draw(){
	// ofCircle(xRes/2, yRes/2, speakers.radius);
	dla.draw(windowY, windowX);

}
//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {

  static float gFL , gFR;
  static float gRL, gRR;

  // *** GAIN MUTEX START ***
  gainMutex.lock();
  gFL = gainFL;
  gFR = gainFR;
  gRL = gainRL;
  gRR = gainRR;	
  gainMutex.unlock();
  // *** GAIN MUTEX END ***

  for(int i = 0; i < bufferSize * nChannels; i += nChannels) {
    float sample = sin(phase); 
    output[i] = gFR*sample; 
    output[i+1] = gFL*sample; 
    output[i+2] = gRL*sample;
	output[i+3] = gRR*sample;  
	phase += 0.01;
  }
}
//--------------------------------------------------------------


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	switch(key)
	{
		case 'q': 
		std::exit(0);
		case 'a':
			animate = !animate;
			break;
		case 's':
			dla.update();
			break;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

	// *** MOUSE MUTEX START ***
	mouseMutex.lock();
	mouseX = x;
	mouseY = y;
	mouseMutex.unlock();
	// ** MOUSE MUTEX END ***

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
