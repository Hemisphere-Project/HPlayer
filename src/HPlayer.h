#pragma once

#include "ofMain.h"
#include "mediaPlayer.h"
#include "ofxCrypto.h"
#include "ofxArgParser.h"
#include "xmlSettings.h"
#include "oscCom.h"

#define OSCPORT_IN 	9000
#define OSCPORT_OUT	9001
#define OSCHOST_OUT "localhost"

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
		
		//OSC
		oscCom	osc;

		//EVENTS
		char eventString[255];
				
};

