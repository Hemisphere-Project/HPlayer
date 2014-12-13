#include "ofMain.h"
#include "HPlayer.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxArgParser.h"

int main(int argc, const char** argv)
{
	//ARGS
	ofxArgParser::init(argc, argv);
	
	ofSetLogLevel(OF_LOG_NOTICE);

	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
	
	ofBackground(0, 255);
	ofSetFrameRate(25);
	
	ofRunApp( new HPlayer());
}
