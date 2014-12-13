#pragma once

#include "ofxOMXPlayer.h"

struct dims_t {
	int height;
	int width;
	int marginX;
	int marginY;
};

struct params_t {
	string 	name;
	int 	volume;
	bool 	mute;
	int 	blur;
	int 	zoom;
	
};

class omPlayer : public ofxOMXPlayer 
{
	
	public:

		//RUN
		void init(bool textured, bool audioHDMI);
		void basepath(string path);

		void buffer();
		void display();
		
		
		
		//CONTROL
		void load(vector<string> playlist, bool doLoop);
		void play(vector<string> playlist, bool doLoop);
		void play(string file, bool doLoop);
		void play(int index);		
		void play();
		
		void next();
		void prev();
		void pause();
		void resume();
		void stop();
		void seek(int timemilli);
		
		void setName(string name);
		void volume();
		void volume(int v);
		void applyVolume();
		void setMuted(bool mute);
		void setLoop(bool doLoop);
		void setBlur(int blur);
		void setZoom(int zoom);
		
		
		//STATE
		string 	getName();
		int  	getVolumeInt();
		int 	getBlur();
		int 	getZoom();
		bool 	isMuted();	
		bool 	isLoop();
		int 	getCurrentFrameNbr();
		int 	getPositionMs();
		int 	getDurationMs();
		string 	getFile();
		int		playlistSize();

		int timeToFrameMs(int timemilli);
		
	private:
				
		//INTERNAL
		void doplay();

		//APPLY FX
		void clearscreen();
		void blur();
		
		//BUFFER & SHADER
		ofShader noshader, blurH, blurV;
		ofFbo framebuffer, frameblur;				
					
		dims_t			dim;
		params_t		params;	
		vector<ofFile> 	videoFiles;
		int				currentIndex;
		bool			enableLoopingList;

		string basePath;
		
		
		
		
};

