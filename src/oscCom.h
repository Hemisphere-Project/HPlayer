#pragma once
#include "ofxOsc.h"
#include "omPlayer.h"
#include "ofxCrypto.h"
#include "xmlSettings.h"

class oscCom
{
	
	public:
		
		oscCom();

		//CONNECT
		void	connect();
		void	connect(string cmd_profile);

		//COMMAND TRANSLATION
		string cmd(string command);
		char* getIP();
		
		//RECEIVE
		void 	execute(omPlayer* player);
		string	log();
		string 	oscToString(ofxOscMessage m);
		
		//SEND	
		void 	status(omPlayer* player);
		void 	end(string file);

		void statusKXKM(omPlayer* player);
		void ipKXKM(omPlayer* player);
		
		//SETTINGS
		int 	portIN;
		int 	portOUT;
		string 	hostOUT;
		bool 	base64;
		string 	cmdmap;
		
		
	private:
	
		bool 	connected;	
		
		ofxOscSender 	oscSender;	
		ofxOscReceiver  oscListener;
		string 			oscDebug;	

		ofxXmlSettings commander;	
		
};

