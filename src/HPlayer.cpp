/* TODO
play et playloop : support multiple files

find how to jump to frame

add termination handler
*/

#include "HPlayer.h"

//--------------------------------------------------------------
void HPlayer::setup()
{		
	//DEFAULT
	playerName = "HPlayer";
	defaultVolume = 50;
	audioHDMI = false,
	enableInfo = false;
	oscPortIN = OSCPORT_IN;
	oscPortOUT = OSCPORT_OUT;
	oscEnable = true;
	glslEnable = false;	
	base64 = false;
	
	//COMMAND LINE ARGS PARSING
	if (ofxArgParser::hasKey("name")) playerName = ofxArgParser::getValue("name");
	if (ofxArgParser::hasKey("volume")) defaultVolume = ofToInt(ofxArgParser::getValue("volume"));
	if (ofxArgParser::hasKey("ahdmi")) audioHDMI = (ofToInt(ofxArgParser::getValue("ahdmi")) == 1);	
	if (ofxArgParser::hasKey("info")) enableInfo = (ofToInt(ofxArgParser::getValue("info")) == 1);	
	if (ofxArgParser::hasKey("in")) oscPortIN = ofToInt(ofxArgParser::getValue("in"));
	if (ofxArgParser::hasKey("out")) oscPortOUT = ofToInt(ofxArgParser::getValue("out"));	
	if (ofxArgParser::hasKey("osc")) oscEnable = (ofToInt(ofxArgParser::getValue("osc")) == 1);
	if (ofxArgParser::hasKey("glsl")) glslEnable = (ofToInt(ofxArgParser::getValue("glsl")) == 1);
	if (ofxArgParser::hasKey("base64")) base64 = (ofToInt(ofxArgParser::getValue("base64")) == 1);
	
	//SETUP OF
	ofHideCursor();
	ofBackground(0,0,0);
	consoleListener.setup(this);
		
	//CONNECT OSC
	Connected = false;	
	if (oscEnable) this->connect();
	
	//INIT PLAYER
	uxPlayer.init(audioHDMI, glslEnable);
	uxPlayer.volume(defaultVolume);
	lastFrame = 0;
	
	//SEND STATUS
	this->sendStatus();
}

//--------------------------------------------------------------
void HPlayer::update()
{	
	//EXECUTE RECEIVED OSC COMMANDS
	this->execute();
	//if (uxPlayer.isPlaying()) ofLog(OF_LOG_NOTICE,"-HP- frame: "+ofToString(uxPlayer.getCurrentFrame())+" "+ofToString(uxPlayer.getTotalNumFrames()));
	
	//DETECT END / LOOP since Listener in ofxOMX are broken
	if (uxPlayer.isPlaying()) 
	{
		int maxFrame = uxPlayer.getTotalNumFrames()-1;
		int currentFrame = uxPlayer.getCurrentFrame();
		
		if (uxPlayer.autoloop())
			if (currentFrame < lastFrame) this->onVideoLoop();
		
		if (!uxPlayer.autoloop())
			if ((currentFrame == maxFrame) and (lastFrame < maxFrame))
			{
				uxPlayer.stop();
				this->onVideoEnd();
			}
		
		lastFrame = currentFrame;
	}
	else lastFrame = 0;
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

void HPlayer::onVideoEnd()
{
	ofLog(OF_LOG_NOTICE,"-HP- The Media did end");
	
	this->sendStatus();
}

void HPlayer::onVideoLoop()
{
	ofLog(OF_LOG_NOTICE,"-HP- The Media did loop");
	
	this->sendStatus();	
}

void HPlayer::onCharacterReceived(SSHKeyListenerEventData& e)
{
	keyPressed((int)e.character);
}

void HPlayer::keyPressed  (int key){
	
	ofLog(OF_LOG_NOTICE,"-HP- Key pressed: "+key);
	
	switch (key) 
	{
		case 'q':
		{
			uxPlayer.close();
			std::exit(0);
			break;
		}
	}
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
		  	
	  	if ((command == "play") or (command == "playloop"))
		{			
			vector<ofFile> playlist;
			ofFile file;
			string filepath;
			
			bool doLoop = (command == "playloop");
			
			for(int k = 0; k < m.getNumArgs(); k++)
			{
				if ((m.getArgType(k) == OFXOSC_TYPE_STRING) && (m.getArgAsString(k) != "")) 
				{
					filepath = m.getArgAsString(k);
					if (base64) filepath = ofxCrypto::base64_decode(filepath);
					
					file = ofFile::ofFile(filepath);
					if (file.isFile()) playlist.push_back(file);
				}
			}
			
			if (playlist.size() == 1) uxPlayer.play(playlist[0].path(),doLoop);
			else if (playlist.size() > 1) uxPlayer.play(playlist,doLoop);			
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
		else if(command == "quit")
		{
			ofLog(OF_LOG_NOTICE,"-HP- QUIT ");
			uxPlayer.close();
			std::exit(0);
		}
		
		sendStats = true;
		if (enableInfo) oscDebug += this->oscToString(m)+"\n";
		//if (enableInfo) ofLog(OF_LOG_NOTICE,"-HP- OSC Received: "+this->oscToString(m));
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
	if (!Connected) return;
	
	ofxOscMessage m;
	m.setAddress("/status");
	m.addStringArg(playerName);
	
	string filepath = uxPlayer.getFile();
	if (base64) filepath = ofxCrypto::base64_encode(filepath);
	
	if (uxPlayer.isPlaying())
	{
		if (uxPlayer.isPaused()) m.addStringArg("paused");
		else m.addStringArg("playing");
		
		m.addStringArg(filepath);
		m.addIntArg(uxPlayer.getPositionMs());
		m.addIntArg(uxPlayer.getDurationMs());
	}
	else 
	{
		m.addStringArg("stoped");
		m.addStringArg(filepath);
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
		info <<"\n" <<	"POSITION / DURATION (): 	"				<< ofToString(uxPlayer.getPositionMs()/1000.) << " / " << ofToString(uxPlayer.getDurationMs()/1000.);	
		info <<"\n" <<	"FRAMES: 	"				<< uxPlayer.getCurrentFrame() << " / " << uxPlayer.getTotalNumFrames();	
	}
	
	info <<"\n" <<	"OSC [IN "<<oscPortIN<<"] [OUT "<<oscPortOUT<<"]: 		"	<< oscDebug;

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}

void HPlayer::displayStandby() {

	stringstream info;
	info << " .:: HPLAYER ::. ";
	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}

