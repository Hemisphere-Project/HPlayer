#include "HPlayer.h"


/**
 * Set up the HPlayer object. This is needed for the interaction with
 * openFrameworks.
 */
void HPlayer::setup()
{
	//OF SETTINGS
	ofHideCursor();
	//
	// //PLAYER SETTINGS
	player.configure("settings.xml", true);

	//OSC SETTINGS
	osc.configure("settings.xml", true);

	// //INITIAL MEDIA && AUTOSTART
	string media = "";
	if (ofxArgParser::hasKey("media")) media = ofxArgParser::getValue("media");
	bool start = true;
	if (ofxArgParser::hasKey("start")) start = (ofToInt(ofxArgParser::getValue("start")) == 1);

	//INIT PLAYER
	player.init();

	//CONNECT OSC
	osc.connect();

	// //AUTOSTART
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


/**
 * Update the internal data of the HPlayer object. It handles the off-screen
 * computation done when interacting with openFrameworks.
 */
void HPlayer::update()
{
	//EXECUTE RECEIVED OSC COMMANDS
	osc.execute(&player);

	//PLAYER UPDATE
	player.update();
}

/**
 * Do the on-screen drawing when interacting with openFrameworks.
 */
void HPlayer::draw()
{
	//PLAYER DRAW
	player.draw();
}


/**
 * Callback for openFrameworks to execute upon pressing a (keyboard) key.
 *
 * \param key The keycode of the key pressed
 */
void HPlayer::keyPressed(int key){
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
