#include "oscCom.h"
#include <net/if.h>
#include <arpa/inet.h>
#include <ofFileUtils.h>

oscCom::oscCom()
{
	portIN = 9000;
	portOUT = 9001;
	hostOUT = "localhost";
	base64 = false;
	cmdmap = "default";
}

void oscCom::connect()
{
	oscListener.setup(portIN);
	oscSender.setup(hostOUT, portOUT);
	connected = true;

	//set default commands
	xmlSettings settings("settings.xml");
	settings.conf_str("commands:default","play", "play");
	settings.conf_str("commands:default","playloop", "playloop");
	settings.conf_str("commands:default","load", "load");
	settings.conf_str("commands:default","stop", "stop");
	settings.conf_str("commands:default","pause", "pause");
	settings.conf_str("commands:default","resume", "resume");
	settings.conf_str("commands:default","getStatus", "getStatus");
	settings.conf_str("commands:default","next", "next");
	settings.conf_str("commands:default","prev", "prev");
	settings.conf_str("commands:default","volume", "volume");
	settings.conf_str("commands:default","mute", "mute");
	settings.conf_str("commands:default","unmute", "unmute");
	settings.conf_str("commands:default","loop", "loop");
	settings.conf_str("commands:default","unloop", "unloop");
	settings.conf_str("commands:default","blur", "blur");
	settings.conf_str("commands:default","zoom", "zoom");
	settings.conf_str("commands:default","info", "info");
	settings.conf_str("commands:default","host", "host");
	settings.conf_str("commands:default","quit", "quit");
	

	//load config
	commander.loadFile("settings.xml");
	commander.pushTag("commands");
	commander.pushTag(cmdmap);
}

string oscCom::cmd(string command)
{
	return commander.getValue(command,command);
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
		  	
	  	if ((command == cmd("play")) or (command == cmd("playloop")) or (command == cmd("load")))
		{			
			vector<string> playlist;
			string filepath;
			
			bool doLoop = (command == cmd("playloop"));
			
			for(int k = 0; k < m.getNumArgs(); k++)
				if ((m.getArgType(k) == OFXOSC_TYPE_STRING) && (m.getArgAsString(k) != "")) 
				{
					filepath = m.getArgAsString(k);
					if (base64) filepath = ofxCrypto::base64_decode(filepath);	
								
					playlist.push_back(filepath);
				}
			
			if (m.getNumArgs() > 0) player->load(playlist,doLoop);
			if (command != cmd("load")) player->play();							
		}
		else if(command == cmd("stop"))
		{
			player->stop();
		}
		else if(command == cmd("pause"))
		{
			player->pause();
		}
		else if(command == cmd("resume"))
		{
			player->resume();
		}	
		else if(command == cmd("next"))
		{
			player->next();
		}
		else if(command == cmd("prev"))
		{
			player->prev();
		}
		else if(command == cmd("seek"))
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->seek(m.getArgAsInt32(0));
		}			
		else if(command == cmd("volume"))
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->volume(m.getArgAsInt32(0));
		}
		else if(command == cmd("mute"))
		{
			bool doMute = true;
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_INT32)) doMute = (m.getArgAsInt32(0) == 1);
		
			player->setMuted(doMute);
		}
		else if(command == cmd("unmute"))
		{
			player->setMuted(false);
		}
		else if(command == cmd("loop"))
		{
			bool doLoop = true;
			if ((m.getNumArgs() > 0) && (m.getArgType(0) == OFXOSC_TYPE_INT32)) doLoop = (m.getArgAsInt32(0) == 1);
		
			player->setLoop(doLoop);
		}
		else if(command == cmd("unloop"))
		{
			player->setLoop(false);
		}
		else if(command == cmd("blur"))
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->setBlur(m.getArgAsInt32(0));
		}
		else if(command == cmd("zoom"))
		{
			if(m.getArgType(0) == OFXOSC_TYPE_INT32) player->setZoom(m.getArgAsInt32(0));
		}			
		else if(command == cmd("info"))
		{
			//enableInfo = !enableInfo;
		}
		else if(command == cmd("host"))
		{
			if ((m.getArgType(0) == OFXOSC_TYPE_STRING) && (m.getArgAsString(0) != "")) hostOUT = m.getArgAsString(0);
		}
		else if(command == cmd("getStatus"))
		{
			this->status(player);
		}				
		else if(command == cmd("quit"))
		{
			ofLog(OF_LOG_NOTICE,"-HP- QUIT ");
			player->close();
			std::exit(0);
		}
		//KXKM regie
		else if(command == cmd("synctest"))
		{
			this->statusKXKM(player);
		}
		else if(command == cmd("fullsynctest"))
		{
			this->ipKXKM(player);
		}	
		
		oscDebug += this->oscToString(m)+"\n";
    }	
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

char* oscCom::getIP()
{
	int fd;
    struct ifreq ifr;
    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    char interface[] = "eth0";
    strncpy(ifr.ifr_name, interface, IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    return(inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
}


//KXKM centralized control specific
void oscCom::statusKXKM(omPlayer* player) 
{
	if (!connected) return;
	
	ofxOscMessage m;
	m.setAddress("/"+player->getName());

	m.addStringArg("auto");  //auto or manu => external state ctrl
	m.addStringArg( (player->isLoop()) ? "loop" : "unloop" );
	m.addStringArg("screen"); //screen or noscreen
	//m.addStringArg("normal"); //normal or faded
	
	
	if (player->isPlaying())
	{
		m.addStringArg("playmovie");
		m.addStringArg(ofFilePath::getFileName(player->getFile()));
	}
	else m.addStringArg("stopmovie");
	
	oscSender.sendMessage(m);
}

void oscCom::ipKXKM(omPlayer* player) 
{
	if (!connected) return;
	
	ofxOscMessage m;
	m.setAddress("/"+player->getName());

	m.addStringArg("initinfo"); 
	m.addStringArg(this->getIP());
	
	oscSender.sendMessage(m);
}


