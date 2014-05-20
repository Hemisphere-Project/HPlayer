#pragma once
#include "ofxOsc.h"
#include "omPlayer.h"
#include "ofxCrypto.h"

class oscCom
{
	
	public:
		
		//CONNECT
		void	connect();
		
		//RECEIVE
		void 	execute(omPlayer* player);
		string	log();
		string 	oscToString(ofxOscMessage m);
		
		//SEND	
		void 	status(omPlayer* player);
		void 	end(string file);
		
		//SETTINGS
		int 	portIN;
		int 	portOUT;
		string 	hostOUT;
		bool 	base64;
		
		
	private:
	
		bool 	connected;	
		
		ofxOscSender 	oscSender;	
		ofxOscReceiver  oscListener;
		string 			oscDebug;		
		
};

