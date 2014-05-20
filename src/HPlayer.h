#pragma once

#include "ofMain.h"
#include "omPlayer.h"
#include "ofxCrypto.h"
#include "ofxArgParser.h"
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
	
	private:
	
		//HPLAYER
		string playerName;
		bool	enableInfo;	
	
		//PLAYER
		omPlayer player;	
		int 	lastFrame;
		int		freeze;
		
		//OSC
		oscCom	osc;
		
		//DISPLAY TOOLS
		void 	displayInfo();
		void 	displayStandby();
		
		
};

