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
		void	connect(string cmd_profile); /*!< \todo implementation (?) */

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
		int 	portIN; /*!< local port to listen for incoming data */
		int 	portOUT; /*!< remote port to send data to */
		string 	hostOUT; /*!< remote IP to send data to */
		bool 	base64; /*!< indicate (need for) base64 encoding of data */
		string 	cmdmap; /*!< mapping of command strings to OSC */
		string  prefix; /*!< (internal) prefix for OSC messages */
		
		
	private:

		bool 	connected;	/*!< indicate active OSC network setup (sender & reciver) */
		ofxOscSender 	oscSender;	/*!< sender of OSC info [details](http://openframeworks.cc/documentation/ofxOsc/ofxOscSender.html) */
		ofxOscReceiver  oscListener; /*!< receiver of OSC info [details](http://openframeworks.cc/documentation/ofxOsc/ofxOscReceiver.html) */
		string 			oscDebug;	/*!< OSC debug messages */
		ofxXmlSettings commander;	/*!< main configuration */
		
};

