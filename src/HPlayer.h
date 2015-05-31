#pragma once

#include "ofMain.h"
#include "mediaPlayer.h"
#include "ofxCrypto.h"
#include "ofxArgParser.h"
#include "xmlSettings.h"
#include "oscCom.h"


/**
 * The basic class to handle the interaction with
 * openFrameworks.
 */
class HPlayer : public ofBaseApp
{	
	public:
	
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
	private:

		mediaPlayer player; /*!< container to manage content */
		oscCom	osc; /*!< OSC communication class */
		char eventString[255]; /*!< event (description) going on */
};
