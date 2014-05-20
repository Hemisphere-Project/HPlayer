#pragma once

#include "ofxOMXPlayer.h"

struct dims_t {
	int height;
	int width;
	int marginX;
	int marginY;
};

struct fx_t {
	int blur;
};

class omPlayer : public ofxOMXPlayer 
{
	
	public:

		//RUN
		void init();
		void buffer();
		void display();
		
		//FX
		void blur();
		
		//BUFFER & SHADER
		ofShader noshader, blurH, blurV;
		ofFbo framebuffer, frameblur;
		
		//CONTROL
		void play(vector<string> playlist, bool doLoop);
		void play(string file, bool doLoop);
		void play(int index);		
		void play();
		
		void next();
		void prev();
		void pause();
		void resume();
		void stop();
		
		void volume();
		void volume(int v);
		void setMuted(bool mute);
		void setLoop(bool doLoop);
		
		
		//STATE
		int  	getVolumeInt();
		bool 	isMuted();	
		bool 	isLoop();
		int 	getCurrentFrameNbr();
		int 	getPositionMs();
		int 	getDurationMs();
		string 	getFile();
		int		playlistSize();

		//SETTINGS
		bool audioHDMI;
		
	private:
					
		dims_t			dim;
		fx_t			fx;		
		int 			currentVolume;
		bool 			muteVolume;
		vector<ofFile> 	videoFiles;
		int				currentIndex;
		bool			enableLoopingList;
		
		
		
		
};

