#include "ofMain.h"
#include "HPlayer.h"
#include "ofGLProgrammableRenderer.h"
#include "ofxArgParser.h"

int main(int argc, const char** argv)
{
	
	//ARGS
	ofxArgParser::init(argc, argv);
	
	ofSetLogLevel(OF_LOG_NOTICE);
	//ofSetLogLevel(OF_LOG_WARNING);

	ofLog(OF_LOG_NOTICE,"-HP- WOKE UP ");

	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1280, 720, OF_FULLSCREEN);
	
	ofBackground(0, 255);
	ofSetFrameRate(30);
	
	ofRunApp( new HPlayer());
}
