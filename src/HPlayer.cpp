#include "HPlayer.h"

//--------------------------------------------------------------
void HPlayer::setup()
{
	//OF SETTINGS
	ofHideCursor();
	
	//HPLAYER SETTINGS
	playerName = "HPlayer";
	enableInfo = false;
	
	if (ofxArgParser::hasKey("name")) playerName = ofxArgParser::getValue("name");
	if (ofxArgParser::hasKey("info")) enableInfo = (ofToInt(ofxArgParser::getValue("info")) == 1);	
	
	
	//SETTINGS OSC
	osc.portIN 	= OSCPORT_IN;
	osc.portOUT = OSCPORT_OUT;
	osc.hostOUT = OSCHOST_OUT;
	osc.base64 	= false;
	
	if (ofxArgParser::hasKey("in")) osc.portIN = ofToInt(ofxArgParser::getValue("in"));
	if (ofxArgParser::hasKey("out")) osc.portOUT = ofToInt(ofxArgParser::getValue("out"));	
	if (ofxArgParser::hasKey("base64")) osc.base64 = (ofToInt(ofxArgParser::getValue("base64")) == 1);
	
	//CONNECT OSC
	osc.connect();
	
	//PLAYER SETTINGS	
	int defaultVolume = 50;
	bool loop = true;
	player.audioHDMI = false;

	if (ofxArgParser::hasKey("volume")) defaultVolume = ofToInt(ofxArgParser::getValue("volume"));
	if (ofxArgParser::hasKey("ahdmi")) player.audioHDMI = (ofToInt(ofxArgParser::getValue("ahdmi")) == 1);
	if (ofxArgParser::hasKey("loop")) loop = (ofToInt(ofxArgParser::getValue("loop")) == 1);	
	
	//INIT PLAYER
	player.init();
	player.volume(defaultVolume);
	lastFrame = 0;
	freeze = 0;
	
	//AUTOSTART WITH MEDIA PATH
	if (ofxArgParser::hasKey("media")) 
	{
		vector<string> playlist;
		playlist.push_back(ofxArgParser::getValue("media"));
		player.play( playlist, loop);		
	}
	
	//SEND STATUS
	osc.status(&player);

}



//--------------------------------------------------------------
void HPlayer::update()
{
	//EXECUTE RECEIVED OSC COMMANDS
	osc.execute(&player);
	
	//BUFFER FRAME
	player.buffer();
	
	//TODO BRING THAT IN THE uxOMX
	//DETECT END / LOOP since Listener in ofxOMX are broken
	if (player.isPlaying()) 
	{
		int maxFrame = player.getTotalNumFrames()-1;
		int currentFrame = player.getCurrentFrameNbr();
		
		//FILE REACH THE END
		if ((currentFrame == maxFrame) and (lastFrame < maxFrame)) 
		{			
			player.next(); 
			osc.status(&player); //TODO Create event statut change
		}		
		
		//FREEZE detection (due to wrong frame counter)
		if ((currentFrame == lastFrame) && (!player.isPaused())) 
		{
			freeze++;
			if (freeze > 10) player.next();
		}
		else freeze = 0;
		
			
		lastFrame = currentFrame;
	}
	else lastFrame = 0;
	
}

//--------------------------------------------------------------
void HPlayer::draw(){
	
	player.display();
	
	//STANDBY MESSAGE
	if (!player.isPlaying()) this->displayStandby();
	
	//DEBUG INFO DISPLAY
	if (enableInfo) this->displayInfo();
}



//--------------------------------------------------------------
void HPlayer::displayInfo() {

	stringstream info;
	info << "------- HPLAYER INFO -----------\n";
	info <<"\n" << "NAME " << playerName;
	info <<"\n" << "APP FPS: 	"+ ofToString(ofGetFrameRate());
	info <<"\n" <<	"IS PLAYING: 	"			<< std::boolalpha << player.isPlaying();
	info <<"\n" <<	"IS PAUSED: 	"			<< std::boolalpha << player.isPaused();
	info <<"\n" <<	"IS OPEN: 	"			<< std::boolalpha << player.isOpen;
	info <<"\n" <<	"VOLUME: 	"				<< player.getVolumeInt();
	
	if (player.isOpen)
	{
		info <<"\n" <<	"DIMENSIONS: 	"			<< player.getWidth()<<"x"<<player.getHeight();
		info <<"\n" <<	"POSITION / DURATION (): 	"				<< ofToString(player.getPositionMs()/1000.) << " / " << ofToString(player.getDurationMs()/1000.);	
		info <<"\n" <<	"FRAMES: 	"				<< player.getCurrentFrameNbr() << " / " << player.getTotalNumFrames();	
	}
	
	info <<"\n" << osc.log();

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}

//--------------------------------------------------------------
void HPlayer::displayStandby() {

	stringstream info;
	info << " .:: HPLAYER ::. ";
	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}
