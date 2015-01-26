#pragma once
#include "ofxOsc.h"
#include "mediaPlayer.h"
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
		void 	execute(mediaPlayer* player);
		string	log();
		string 	oscToString(ofxOscMessage m);
		
		//SEND	
		void 	status(mediaPlayer* player);
		void 	status(mediaPlayer* player, string postman);
		void 	end(string file);

		void statusKXKM(mediaPlayer* player);
		void ipKXKM(mediaPlayer* player);
		
		//SETTINGS
		int 	portIN;
		int 	portOUT;
		string 	hostOUT;
		bool 	base64;
		string 	cmdmap;
		string  prefix;
		
		
	private:
	
		bool 	connected;	
		
		ofxOscSender 	oscSender;	
		ofxOscReceiver  oscListener;
		string 			oscDebug;	

		ofxXmlSettings commander;	
		
};

