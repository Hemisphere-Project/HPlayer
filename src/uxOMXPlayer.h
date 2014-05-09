#pragma once

#include "ofxOMXPlayer.h"

#define VIDEO_DIR 	""

struct dims_t {
  int height;
  int width;
  int marginX;
  int marginY;
};

class uxOMXPlayer : public ofxOMXPlayer
{
	public:

		void init(bool audioHDMI, bool autoLoop, bool glsl);
		void display();	
		
		void play(vector<string> playlist, bool doLoop);
		void play(string file, bool doLoop);
		void play(int index);		
		void play();
		
		void next();
		void prev();
		void pause();
		void resume();
		void stop();
	
		void setLoop(bool doLoop);
		bool isLoop();
		
		void volume();
		void volume(int v);
		void volume(double v);
		void setMuted(bool mute);
		
		string 	getFile();
		int		playlistSize();
		int  	getVolumeInt();
		int 	getPositionMs();
		int 	getDurationMs();
		bool 	isMuted();		
	
	private:
		ofxOMXPlayerSettings settings;	
		dims_t	dim;
	
		vector<ofFile> 	videoFiles;
		int				currentIndex;
		bool			enableLoopingList;
					
		float 		currentVolume;
		bool 		muteVolume;
		
		bool		enableGLSL;
		ofShader 	shader;
		ofFbo 		fbo;	
};

