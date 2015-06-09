#include "ofApp.h"
#include <assert.h>

int xRes = 500;
int yRes = 500;

int windowX = xRes;
int windowY = yRes;

int numParticles = 50;
float particle_size = 2;

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
	speakers.radius = xRes/32.0; // Radius of the speaker setup in pixels
	speakers.quadSetup();	// Setup quadraphonic system
	phase = 0;
	ofSoundStreamSetup(4, 0); // 4 output channels, 0 input channels

	particle_system.init(xRes, yRes, numParticles);
	particle_system.drawRadius = particle_size;
	particle_system.clearanceRadius = speakers.radius;

	for (int x1=0; x1<xRes; x1++)
	for (int y1=0; y1<yRes; y1++)
	{
		ofVec2f location(x, y);

		if(location.length()<=3*speakers.radius)
		{
			ofVec2f v(x, y);
			v *= 5;
			particle_system.velocityField.set(x, y, v);
		}

		else
		{
			ofVec2f v(x*x, y*y);
			v = v*2.0/v.length();
			particle_system.velocityField.set(x, y, v);
		}
	}


}

//--------------------------------------------------------------
void ofApp::update(){

	int xPos, yPos;
	
	xPos = xPos - xRes*0.5;
	yPos = - yPos + yRes*0.5;

	ofVec2f d(xPos, yPos);
	float distGain = compute_dist_gain(d, speakers.radius);

    // *********************** GAIN MUTEX START ***********************
	gainMutex.lock();
	gainFR = compute_azimuth_gain(d, speakers.channelFR());				//
	gainFL = compute_azimuth_gain(d, speakers.channelFL());				//
	gainRL = compute_azimuth_gain(d, speakers.channelRL());				//
	gainRR = compute_azimuth_gain(d, speakers.channelRR());				//

	// Normalize gains
	float norm = magnitude4f(gainFR, gainFL, gainRL, gainRR);
	gainFR /= norm;														//
	gainFL /= norm;														//
	gainRL /= norm;														//
	gainRR /= norm;														//

	// Distance-based gain
	gainFR *= distGain;													// 
	gainFL *= distGain;													// 
	gainRL *= distGain;													// 
	gainRR *= distGain;													//	

	gainMutex.unlock();
	// ************************* GAIN MUTEX END ************************
	// cout << gainFR << " " << 
	// 	    gainFL << " " <<
	//  		gainRL << " " <<
	//  		gainRR << " " << endl;
	// cout << "    Power: "<<  magnitude4f(gainFR, gainFL, gainRL, gainRR) << endl;

	if(animate)
	{
		particle_system.update();
	}


}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofCircle(xRes/2, yRes/2, speakers.radius);

	particle_system.draw();
	
}
//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {

  static float gFL , gFR;
  static float gRL, gRR;

  // **** GAIN MUTEX START ****
  gainMutex.lock();
  gFL = gainFL;				//
  gFR = gainFR;				//
  gRL = gainRL;				//
  gRR = gainRR;				//	
  gainMutex.unlock();
  // **** GAIN MUTEX END *****

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
			particle_system.update();
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

	cout << particle_system.velocityField(x, y) << endl;

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
