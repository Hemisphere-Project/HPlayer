#pragma once

#include "ofMain.h"
#include "uxOMXPlayer.h"
#include "ofxOsc.h"

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
	
		//OSC RECEIVER
		ofxOscReceiver  oscListener;
		int 			oscPortIN;
		bool			Connected;
		string 			oscDebug;
		void 	connect();
		void 	execute();
		string 	oscToString(ofxOscMessage m);
		
		//OSC SENDER
		ofxOscSender 	oscSender;
		int 			oscPortOUT;
		void 	sendStatus();
		void 	sendEnd(string file);
		
		//DISPLAY TOOLS
		void 	displayInfo();
		void 	displayStandby();
		
		//PLAYER
		uxOMXPlayer		uxPlayer;
		bool 			glslEnable;
		bool			enableInfo;	
		string 			playerName;
		int 			defaultVolume;
		bool 			oscEnable;	
		
};

