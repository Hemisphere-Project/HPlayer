#pragma once

#include "ofMain.h"
#include "uxOMXPlayer.h"
#include "ofxOsc.h"
#include "ofxCrypto.h"
#include "ofxArgParser.h"
#include "ConsoleListener.h"

#define OSCPORT_IN 	9000
#define OSCPORT_OUT	9001
#define OSCHOST_OUT "localhost"

class HPlayer : public ofBaseApp, public SSHKeyListener
{
	public:

		void setup();
		void update();
		void draw();
		
		void keyPressed(int key);
		
		//SSH KEYPRESS RECEIVER
		void 	onCharacterReceived(SSHKeyListenerEventData& e);
		
	
	private:
	
		//SSH LISTENER
		ConsoleListener consoleListener;
	
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
		bool	base64;
		void 	sendStatus();
		void 	sendEnd(string file);
		
		//DISPLAY TOOLS
		void 	displayInfo();
		void 	displayStandby();
		
		//PLAYER
		uxOMXPlayer		uxPlayer;
		bool 			autoLoop;
		bool 			glslEnable;
		bool			enableInfo;	
		bool			audioHDMI;	
		string 			playerName;
		int 			defaultVolume;
		bool 			oscEnable;	
		int 			lastFrame;
		int				freeze;
		
};

