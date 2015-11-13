#include "ofMain.h"
#include "HPlayer.h"
//#include "ofGLProgrammableRenderer.h" // OF0.8
#include "ofxArgParser.h"

int main(int argc, const char** argv)
{

	//ARGS
	ofxArgParser::init(argc, argv);

	//ofSetLogLevel(OF_LOG_NOTICE);
	ofSetLogLevel(OF_LOG_WARNING);

	ofLog(OF_LOG_WARNING,"Welcome to HPlayer !");

	// OF0.8
	//ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);

	ofSetupOpenGL(1280, 720, OF_FULLSCREEN);// <-------- setup the GL context

	ofBackground(0, 255);
	ofSetFrameRate(30);

	ofRunApp( new HPlayer());
}
