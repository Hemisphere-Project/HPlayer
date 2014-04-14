#include "ofMain.h"
#include "ofGLProgrammableRenderer.h"
#include "HPlayer.h"
#include "ofxArgParser.h"

int main(int argc, const char** argv)
{
	//ARGS
	ofxArgParser::init(argc, argv);
	
	//INIT GRAPHICS
	ofSetLogLevel(OF_LOG_NOTICE);
	ofSetCurrentRenderer(ofGLProgrammableRenderer::TYPE);
	ofSetupOpenGL(1280, 720, OF_WINDOW);
	
	//START APP	
	ofRunApp(new HPlayer());
}


