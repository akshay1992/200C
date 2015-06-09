#include "ofApp.h"
#include <assert.h>

int xRes = 500;
int yRes = 500;

int windowX = xRes;
int windowY = yRes;

int numParticles = 500;
float particle_size = 2;

int animate = 0;

//--------------------------------------------------------------

void ofApp::setup(){
	ofSoundStreamSetup(4, 0); // 4 output channels, 0 input channels

	particle_system.drawRadius = particle_size;
	particle_system.init(xRes, yRes, numParticles);
}

//--------------------------------------------------------------
void ofApp::update(){

	if(animate)
	{
		particle_system.update();
	}


}
//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0);
	ofCircle(xRes/2, yRes/2, particle_system.speakers.radius);

	particle_system.draw();
	
}
//--------------------------------------------------------------
void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {

  for(int i = 0; i < bufferSize * nChannels; i += nChannels) {
  	for(int p=0; p<particle_system.numParticles; p++)
  	{
  		float sample = sin(particle_system.particle[p].phase);
  		particle_system.particle[p].gainMutex.lock();
  		for(int channel=0; channel<4; channel++)
  			output[i+channel] = particle_system.particle[p].gain[channel]*sample; 
  		particle_system.particle[p].gainMutex.unlock();
		particle_system.particle[p].phase += particle_system.particle[p].phase_increment;
  	}
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
