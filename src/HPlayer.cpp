#include "HPlayer.h"
#include "ofxArgParser.h"

//--------------------------------------------------------------
void HPlayer::setup()
{		
	//DEFAULT
	playerName = "HPlayer";
	defaultVolume = 50;
	enableInfo = true;
	oscPortIN = OSCPORT_IN;
	oscPortOUT = OSCPORT_OUT;
	oscEnable = true;
	glslEnable = false;
	
	
	
	//COMMAND LINE ARGS PARSING
	if (ofxArgParser::hasKey("name")) playerName = ofxArgParser::getValue("name");
	if (ofxArgParser::hasKey("volume")) defaultVolume = ofToInt(ofxArgParser::getValue("volume"));
	if (ofxArgParser::hasKey("info")) enableInfo = true;	
	if (ofxArgParser::hasKey("in")) oscPortIN = ofToInt(ofxArgParser::getValue("in"));
	if (ofxArgParser::hasKey("out")) oscPortOUT = ofToInt(ofxArgParser::getValue("out"));	
	if (ofxArgParser::hasKey("noosc")) oscEnable = false;
	if (ofxArgParser::hasKey("glsl")) glslEnable = true;
	
	//SETUP OF
	ofHideCursor();
	ofBackground(0,0,0);
		
	//CONNECT OSC
	Connected = false;	
	if (oscEnable) this->connect();
	
	//INIT PLAYER
	uxPlayer.init(glslEnable);
	uxPlayer.volume(defaultVolume);
	
	//AUTOSTART PLAYER IF NO OSC
	if (!oscEnable) uxPlayer.play();
}

//--------------------------------------------------------------
void HPlayer::update()
{	
	//EXECUTE RECEIVED OSC COMMANDS
	this->execute();
}

//--------------------------------------------------------------
void HPlayer::draw(){
	
	//DRAW VIDEO
	uxPlayer.display();	
	
	//STANDBY MESSAGE
	if (!uxPlayer.isPlaying()) this->displayStandby();
	
	//DEBUG INFO DISPLAY
	if (enableInfo) this->displayInfo();
}


void HPlayer::connect()
{
	//OSC LISTENER INIT
	oscListener.setup( oscPortIN );
	
	//OSC SENDER INIT
	oscSender.setup(OSCHOST_OUT, OSCPORT_OUT);
	
	Connected = true;
}

void HPlayer::execute()
{
	if (!Connected) return;
	
	bool sendStats = false;
	if (oscListener.hasWaitingMessages()) oscDebug = "";
	
	//OSC GET
	while( oscListener.hasWaitingMessages() ) 
	{
        ofxOscMessage m;
        oscListener.getNextMessage( &m );
        
        vector<string> address = ofSplitString(m.getAddress(),"/");
   		string command = address[1];
   		if (command == "*") command = address[2];
		  	
	  	if (command == "play")
		{			
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_STRING) && (m.getArgAsString(0) != "")) uxPlayer.play(m.getArgAsString(0));
			else uxPlayer.play();
			
		}
		else if(command == "stop")
		{
			uxPlayer.stop();
		}
		else if(command == "pause")
		{
			uxPlayer.pause();
		}
		else if(command == "resume")
		{
			uxPlayer.resume();
		}	
		else if(command == "next")
		{
			uxPlayer.next();
		}	
		else if(command == "volume")
		{
			if (m.getArgType(0) == OFXOSC_TYPE_FLOAT) uxPlayer.volume(m.getArgAsFloat(0));
			else if(m.getArgType(0) == OFXOSC_TYPE_INT32) uxPlayer.volume(m.getArgAsInt32(0));
		
			//if (uxPlayer.isMuted()) uxPlayer.setMuted(false);
		}
		else if(command == "mute")
		{
			bool doMute = true;
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_FLOAT)) doMute = (m.getArgAsFloat(0) == 1.0);
		
			uxPlayer.setMuted(doMute);
		}
		else if(command == "unmute")
		{
			uxPlayer.setMuted(false);
		}
		else if(command == "info")
		{
			enableInfo = !enableInfo;
		}
		else if	(command == "s")
		{
			//if (subcommand == "getStatus") sendStats = true;
			//else sendStats = false;		
		}			
		else if(command == "quit")
		{
			uxPlayer.close();
			std::exit(0);
		}
		
		sendStats = true;
		if (enableInfo) oscDebug += this->oscToString(m)+"\n";
		if (enableInfo) ofLog(OF_LOG_NOTICE,"-HP- OSC Received: "+this->oscToString(m));
    }	
    
    if (sendStats) this->sendStatus();  
}

string HPlayer::oscToString(ofxOscMessage m) {
	
	string message = m.getAddress()+" ";
	for(int i = 0; i < m.getNumArgs(); i++)
	{
		message += m.getArgTypeName(i);
		message += ": ";

		if(m.getArgType(i) == OFXOSC_TYPE_INT32) 		message += ofToString(m.getArgAsInt32(i));
		else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT) 	message += ofToString(m.getArgAsFloat(i));
		else if(m.getArgType(i) == OFXOSC_TYPE_STRING) 	message += m.getArgAsString(i);
		else message += " unknown";
	}
	
	return message;
}

void HPlayer::sendStatus() 
{
	ofxOscMessage m;
	m.setAddress("/status");
	m.addStringArg(playerName);
	
	if (uxPlayer.isPlaying())
	{
		if (uxPlayer.isPaused()) m.addStringArg("paused");
		else m.addStringArg("playing");
		
		m.addStringArg(uxPlayer.getFile());
		m.addIntArg(uxPlayer.getPositionMs());
		m.addIntArg(uxPlayer.getDurationMs());
	}
	else 
	{
		m.addStringArg("stoped");
		m.addStringArg(uxPlayer.getFile());
		m.addIntArg(0);
		m.addIntArg(uxPlayer.getDurationMs());
	}
	
	m.addIntArg(uxPlayer.getVolumeInt());
	
	if (uxPlayer.isMuted()) m.addStringArg("muted");
	else m.addStringArg("unmuted");
	
	oscSender.sendMessage(m);
}

void HPlayer::sendEnd(string file) 
{
	ofxOscMessage m;
	m.setAddress("/end");
	m.addStringArg(file);
	oscSender.sendMessage(m);
}

void HPlayer::displayInfo() {

	stringstream info;
	info << "------- HPLAYER INFO -----------\n";
	info <<"\n" << "NAME " << playerName;
	info <<"\n" << "APP FPS: 	"+ ofToString(ofGetFrameRate());
	info <<"\n" <<	"IS PLAYING: 	"			<< std::boolalpha << uxPlayer.isPlaying();
	info <<"\n" <<	"IS PAUSED: 	"			<< std::boolalpha << uxPlayer.isPaused();
	info <<"\n" <<	"IS OPEN: 	"			<< std::boolalpha << uxPlayer.isOpen;
	info <<"\n" <<	"VOLUME: 	"				<< uxPlayer.getVolume();
	
	if (uxPlayer.isOpen)
	{
		info <<"\n" <<	"DIMENSIONS: 	"			<< uxPlayer.getWidth()<<"x"<<uxPlayer.getHeight();
		info <<"\n" <<	"DURATION: 	"			<< uxPlayer.getDuration();
		info <<"\n" <<	"FRAMES: 	"				<< uxPlayer.getCurrentFrame() << " / " << uxPlayer.getTotalNumFrames();	
	}
	
	info <<"\n" <<	"OSC [IN "<<oscPortIN<<"] [OUT "<<oscPortOUT<<"]: 		"	<< oscDebug;

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}

void HPlayer::displayStandby() {

	stringstream info;
	info << " HPLAYER :: Hemisphere oscPlayer ";
	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::green);
	
}

