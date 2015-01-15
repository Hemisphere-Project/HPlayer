#include "HPlayer.h"

//--------------------------------------------------------------
void HPlayer::setup()
{
	//OF SETTINGS
	ofHideCursor();
	
	//HPLAYER SETTINGS
	xmlSettings settings("settings.xml");

	playerName = settings.conf_str("system","playerName","HPlayer");	
	enableInfo = settings.conf_bool("system","enableInfo",true);

	int defaultVolume = settings.conf_int("player","volume",50);
	bool audioHDMI = settings.conf_bool("player","audioHdmi",false);
	bool textured = settings.conf_bool("player","textured",true);
	int defaultZoom = settings.conf_int("player","zoom",100);
	int defaultBlur = settings.conf_int("player","blur",0);

	string path = "";
	path = settings.conf_str("media","path","/home/pi/media");
	bool loop = settings.conf_bool("media","loop",true);

	osc.portIN 	= settings.conf_int("osc","portIn",OSCPORT_IN);
	osc.portOUT = settings.conf_int("osc","portOut",OSCPORT_OUT);
	osc.hostOUT = settings.conf_str("osc","hostOut",OSCHOST_OUT);
	osc.base64 = settings.conf_bool("osc","base64",false);
	osc.cmdmap = settings.conf_str("osc","commands","default");

	//PLAYER SETTINGS COMMAND LINE
	if (ofxArgParser::hasKey("name")) playerName = ofxArgParser::getValue("name");
	if (ofxArgParser::hasKey("info")) enableInfo = (ofToInt(ofxArgParser::getValue("info")) == 1);	
	if (ofxArgParser::hasKey("volume")) defaultVolume = ofToInt(ofxArgParser::getValue("volume"));
	if (ofxArgParser::hasKey("ahdmi")) audioHDMI = (ofToInt(ofxArgParser::getValue("ahdmi")) == 1);
	if (ofxArgParser::hasKey("zoom")) defaultZoom = ofToInt(ofxArgParser::getValue("zoom"));
	if (ofxArgParser::hasKey("blur")) defaultBlur = ofToInt(ofxArgParser::getValue("blur"));
	if (ofxArgParser::hasKey("gl")) textured = (ofToInt(ofxArgParser::getValue("gl")) == 1);

	//COMMAND WITH MEDIA PATH
	if (ofxArgParser::hasKey("media")) path = ofxArgParser::getValue("media");
	if (ofxArgParser::hasKey("loop")) loop = (ofToInt(ofxArgParser::getValue("loop")) == 1);

	//SETTINGS OSC COMMAND LINE
	if (ofxArgParser::hasKey("in")) osc.portIN = ofToInt(ofxArgParser::getValue("in"));
	if (ofxArgParser::hasKey("out")) osc.portOUT = ofToInt(ofxArgParser::getValue("out"));
	if (ofxArgParser::hasKey("host")) osc.hostOUT = ofxArgParser::getValue("host");	
	if (ofxArgParser::hasKey("base64")) osc.base64 = (ofToInt(ofxArgParser::getValue("base64")) == 1);
	if (ofxArgParser::hasKey("cmdmap")) osc.cmdmap = ofxArgParser::getValue("cmdmap");	

	//INIT PLAYER
	player.init(textured,audioHDMI);
	player.setName(playerName);	
	player.basepath(path);
	player.volume(defaultVolume);
	player.setZoom(defaultZoom);
	player.setBlur(defaultBlur);
	lastFrame = 0;
	freeze = 0;

	//CONNECT OSC
	osc.connect();

	//AUTOSTART
	/*if (path != "")
	{
		vector<string> playlist;		
		playlist.push_back(path);
		player.play( playlist, loop);
	}*/
	
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
	info << "- HPLAYER INFO -\n";
	
	info <<"\n" << player.getName();
	info <<"\n" << ofToString(ofGetFrameRate()) + "FPS";
	
	info <<"\n";
	if (player.isPlaying()) info << "PLAYING" ;
	else  info << "STOPPED";
	if (player.isPaused()) info<<"\n" << " - PAUSED";
	info <<"\n";
	if (player.isOpen) info << "OPENED" ;
	else  info << "CLOSED";

	info <<"\n" <<	"VOL " << player.getVolumeInt();
	info <<"\n";
	info <<"\n" <<	ofGetWidth()<<"x"<<ofGetHeight();
	
	if (player.isOpen)
	{
		info <<"\n" <<	player.getWidth()<<"x"<<player.getHeight();
		info <<"\n" <<	ofToString(player.getPositionMs()) << " / " << ofToString(player.getDurationMs());	
		info <<"\n" <<	player.getCurrentFrameNbr() << " / " << player.getTotalNumFrames();	
	}
	info <<"\n";
	info <<"\n" << osc.log();

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}

//--------------------------------------------------------------
void HPlayer::displayStandby() {

	stringstream info;
	info << " .:: "<<player.getName()<<" ::. ";
	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
	
}


//--------------------------------------------------------------
void HPlayer::keyPressed  (int key){ 
	if(key & OF_KEY_MODIFIER){
		switch(key){
		case OF_KEY_LEFT:
			player.prev();
			sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT");
			break;
		case OF_KEY_UP:
			sprintf(eventString, "keyPressed = (%i) %s", key, "UP");
			break;
		case OF_KEY_RIGHT:
			player.next();
			sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT");
			break;
		case OF_KEY_DOWN:
			sprintf(eventString, "keyPressed = (%i) %s", key, "DOWN");
			break;
		case OF_KEY_PAGE_UP:
			sprintf(eventString, "keyPressed = (%i) %s", key, "PAGE UP");
			break;
		case OF_KEY_PAGE_DOWN:
			sprintf(eventString, "keyPressed = (%i) %s", key, "PAGE DOWN");
			break;
		case OF_KEY_HOME:
			sprintf(eventString, "keyPressed = (%i) %s", key, "HOME");
			break;
		case OF_KEY_END:
			sprintf(eventString, "keyPressed = (%i) %s", key, "END");
			break;
		case OF_KEY_INSERT:
			sprintf(eventString, "keyPressed = (%i) %s", key, "INSERT");
			break;
		case OF_KEY_LEFT_SHIFT:
			sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT SHIFT");
			break;
		case OF_KEY_LEFT_CONTROL:
			sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT CONTROL");
			break;
		case OF_KEY_LEFT_SUPER:
			sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT SUPER");
			break;
		case OF_KEY_RIGHT_SHIFT:
			sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT SHIFT");
			break;
		case OF_KEY_RIGHT_CONTROL:
			sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT CONTROL");
			break;
		case OF_KEY_LEFT_ALT:
			sprintf(eventString, "keyPressed = (%i) %s", key, "LEFT ALT");
			break;
        case OF_KEY_RIGHT_ALT:
			sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT ALT");
			break;
		case OF_KEY_RIGHT_SUPER:
			sprintf(eventString, "keyPressed = (%i) %s", key, "RIGHT SUPER");
			break;
		}
	}
}
