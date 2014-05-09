#pragma once

#include "ofxOMXPlayer.h"

#define VIDEO_DIR 	""

struct dims_t {
  int height;
  int width;
  int marginX;
  int marginY
};

class uxOMXPlayer : public ofxOMXPlayer
{
	public:

		void init(bool audioHDMI, bool glsl);
		void display();	
		
		void play(vector<ofFile> playlist, bool loop);
		void play(int index);
		
		void play(string filepath, bool loop);
		
		void play(string file);
		
		void next();
		void pause();
		void resume();
		void stop();
	
		bool autoloop();
		
		void volume();
		void volume(int v);
		void volume(double v);
		void setMuted(bool mute);
		
		string 	getFile();
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

