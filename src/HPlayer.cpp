#include "HPlayer.h"

//--------------------------------------------------------------
void HPlayer::setup()
{
	//OF SETTINGS
	ofHideCursor();
	
	//HPLAYER SETTINGS
	xmlSettings settings("settings.xml");

	player.name = settings.conf_str("system","playerName","HPlayer");	
	player.info = settings.conf_bool("system","enableInfo",true);

	player.volume = settings.conf_int("player","volume",50);
	player.ahdmi = settings.conf_bool("player","audioHdmi",false);
	player.textured = settings.conf_bool("player","textured",true);
	player.zoom = settings.conf_int("player","zoom",100);
	player.blur = settings.conf_int("player","blur",0);

	player.basepath( settings.conf_str("media","path","/home/pi/media") );
	player.loop = settings.conf_bool("media","loop",true);

	osc.portIN 	= settings.conf_int("osc","portIn",OSCPORT_IN);
	osc.portOUT = settings.conf_int("osc","portOut",OSCPORT_OUT);
	osc.hostOUT = settings.conf_str("osc","hostOut",OSCHOST_OUT);
	osc.base64 = settings.conf_bool("osc","base64",false);
	osc.prefix = settings.conf_str("osc","prefix","");
	osc.cmdmap = settings.conf_str("osc","commands","default");

	//PLAYER SETTINGS COMMAND LINE
	if (ofxArgParser::hasKey("name")) player.name = ofxArgParser::getValue("name");
	if (ofxArgParser::hasKey("info")) player.info = (ofToInt(ofxArgParser::getValue("info")) == 1);	
	if (ofxArgParser::hasKey("volume")) player.volume = ofToInt(ofxArgParser::getValue("volume"));
	if (ofxArgParser::hasKey("ahdmi")) player.ahdmi = (ofToInt(ofxArgParser::getValue("ahdmi")) == 1);
	if (ofxArgParser::hasKey("zoom")) player.zoom = ofToInt(ofxArgParser::getValue("zoom"));
	if (ofxArgParser::hasKey("blur")) player.blur = ofToInt(ofxArgParser::getValue("blur"));
	if (ofxArgParser::hasKey("gl")) player.textured = (ofToInt(ofxArgParser::getValue("gl")) == 1);

	//COMMAND WITH MEDIA PATH
	if (ofxArgParser::hasKey("path")) player.basepath( ofxArgParser::getValue("path") );
	string media = "";
	if (ofxArgParser::hasKey("media")) media = ofxArgParser::getValue("media");
	if (ofxArgParser::hasKey("loop")) player.loop = (ofToInt(ofxArgParser::getValue("loop")) == 1);
	bool start = false;
	if (ofxArgParser::hasKey("start")) start = (ofToInt(ofxArgParser::getValue("start")) == 1);

	//SETTINGS OSC COMMAND LINE
	if (ofxArgParser::hasKey("in")) osc.portIN = ofToInt(ofxArgParser::getValue("in"));
	if (ofxArgParser::hasKey("out")) osc.portOUT = ofToInt(ofxArgParser::getValue("out"));
	if (ofxArgParser::hasKey("host")) osc.hostOUT = ofxArgParser::getValue("host");	
	if (ofxArgParser::hasKey("base64")) osc.base64 = (ofToInt(ofxArgParser::getValue("base64")) == 1);
	if (ofxArgParser::hasKey("cmdmap")) osc.cmdmap = ofxArgParser::getValue("cmdmap");	
	if (ofxArgParser::hasKey("prefix")) osc.prefix = ofxArgParser::getValue("prefix");	

	//INIT PLAYER
	player.init();

	//CONNECT OSC
	osc.connect();

	//AUTOSTART
	if (media != "")
	{
		vector<string> playlist;		
		playlist.push_back(media);
		player.play(playlist);
	}
	else if (start)
	{
		player.load();
		player.play();
	}
	
	//SEND STATUS
	osc.status(&player);
}



//--------------------------------------------------------------
void HPlayer::update()
{
	//EXECUTE RECEIVED OSC COMMANDS
	osc.execute(&player);
	
	//PLAYER UPDATE
	player.update();
}

//--------------------------------------------------------------
void HPlayer::draw()
{
	//PLAYER DRAW
	player.draw();
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
