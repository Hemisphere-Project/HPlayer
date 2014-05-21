#include "oscCom.h"


void oscCom::connect()
{
	oscListener.setup(portIN);
	oscSender.setup(hostOUT, portOUT);
	connected = true;
}

void oscCom::execute(omPlayer* player)
{
	if (!connected) return;
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
			vector<string> playlist;
			string filepath;
			
			bool doLoop = (command == "playloop");
			
			for(int k = 0; k < m.getNumArgs(); k++)
				if ((m.getArgType(k) == OFXOSC_TYPE_STRING) && (m.getArgAsString(k) != "")) 
				{
					filepath = m.getArgAsString(k);
					if (base64) filepath = ofxCrypto::base64_decode(filepath);	
								
					playlist.push_back(filepath);
				}
			
			player->play( playlist, doLoop);				
		}
		else if(command == "stop")
		{
			player->stop();
		}
		else if(command == "pause")
		{
			player->pause();
		}
		else if(command == "resume")
		{
			player->resume();
		}	
		else if(command == "next")
		{
			player->next();
		}
		else if(command == "prev")
		{
			player->prev();
		}		
		else if(command == "volume")
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->volume(m.getArgAsInt32(0));
		}
		else if(command == "mute")
		{
			bool doMute = true;
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_INT32)) doMute = (m.getArgAsInt32(0) == 1);
		
			player->setMuted(doMute);
		}
		else if(command == "unmute")
		{
			player->setMuted(false);
		}
		else if(command == "loop")
		{
			bool doLoop = true;
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_INT32)) doLoop = (m.getArgAsInt32(0) == 1);
		
			player->setLoop(doLoop);
		}
		else if(command == "unloop")
		{
			player->setLoop(false);
		}
		else if(command == "blur")
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->setBlur(m.getArgAsInt32(0));
		}
		else if(command == "zoom")
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->setZoom(m.getArgAsInt32(0));
		}			
		else if(command == "info")
		{
			//enableInfo = !enableInfo;
		}			
		else if(command == "quit")
		{
			ofLog(OF_LOG_NOTICE,"-HP- QUIT ");
			player->close();
			std::exit(0);
		}
		
		oscDebug += this->oscToString(m)+"\n";
    }	
    
    this->status(player);  
}

string oscCom::log() {
	return oscDebug;
}

string oscCom::oscToString(ofxOscMessage m) {
	
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

void oscCom::status(omPlayer* player) 
{
	if (!connected) return;
	
	ofxOscMessage m;
	m.setAddress("/status");
	m.addStringArg(player->getName());
	
	string filepath = player->getFile();
	if (base64) filepath = ofxCrypto::base64_encode(filepath);
	
	if (player->isPlaying())
	{
		if (player->isPaused()) m.addStringArg("paused");
		else m.addStringArg("playing");
		
		m.addStringArg(filepath);
		m.addIntArg(player->getPositionMs());
		m.addIntArg(player->getDurationMs());
		m.addIntArg( (player->isLoop()) ? 1 : 0 );
	}
	else 
	{
		m.addStringArg("stoped");
		m.addStringArg(filepath);
		m.addIntArg(0);
		m.addIntArg(player->getDurationMs());
		m.addIntArg( (player->isLoop()) ? 1 : 0 );
	}
	
	m.addIntArg(player->getVolumeInt());
	
	if (player->isMuted()) m.addStringArg("muted");
	else m.addStringArg("unmuted");
	
	m.addIntArg(player->getZoom());
	m.addIntArg(player->getBlur());
	
	oscSender.sendMessage(m);
}

void oscCom::end(string file) 
{
	if (!connected) return;
	
	ofxOscMessage m;
	m.setAddress("/end");
	m.addStringArg(file);
	oscSender.sendMessage(m);
}

