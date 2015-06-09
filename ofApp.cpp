#include "ofApp.h"
#include <assert.h>

int xRes = 500;
int yRes = 500;

int windowX = xRes;
int windowY = yRes;

int numParticles = 3;
float particle_size = 2;

int animate = 0;

//--------------------------------------------------------------

ofVec2f v_field_function(int x, int y)
{
	return ofVec2f(x*x, y*y);

	// if (x!=0 && y!=0)
	// 	return ofVec2f( x, y ) / pow( x*x + y*y , 1.5);
	// else
	// 	return ofVec2f( 2, 2);

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

	for (int x=0; x<xRes; x++)
	for (int y=0; y<yRes; y++)
	{
		ofVec2f location(x, y);
		if(location.length()<=1.5*speakers.radius)
		{
			// TO keep the particles away from the speakers
			ofVec2f velocity = convert(location);
			velocity = convert_back(velocity);

			velocity.normalize();
			velocity *= 500;

			particle_system.velocityField.set(x, y, velocity);
		}

		else
		{
			// Velocity field
			ofVec2f velocity = convert(location);
			velocity[0] = velocity[0] * velocity[0];
			velocity[1] = velocity[1] * velocity[1];
			velocity = convert_back(velocity);

			velocity.normalize();

			particle_system.velocityField.set(x, y, velocity);
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

	cout << "Velocity Field: " << particle_system.velocityField(x, y) << endl;

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
