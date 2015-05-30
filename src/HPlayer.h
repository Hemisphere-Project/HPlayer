#pragma once

#include "ofMain.h"
#include "mediaPlayer.h"
#include "ofxCrypto.h"
#include "ofxArgParser.h"
#include "xmlSettings.h"
#include "oscCom.h"

class HPlayer : public ofBaseApp
{
	
	public:
	
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
	
	private:

		//PLAYER
		mediaPlayer player;

		oscCom	osc; /*!< OSC communication class */
		char eventString[255]; /*!< event (description) going on */
				
};

