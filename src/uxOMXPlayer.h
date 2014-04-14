#pragma once

#include "ofxOMXPlayer.h"

#define VIDEO_DIR 	"video"

class uxOMXPlayer : public ofxOMXPlayer
{
	public:

		void init(bool glsl);
		void display();	
		
		void play();
		void play(int index);
		void play(string file);
		void next();
		void pause();
		void resume();
		void stop();
		
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
		
		vector<ofFile> 	videoFiles;
		int				currentIndex;
		
		float 		currentVolume;
		bool 		muteVolume;
		
		bool		enableGLSL;
		ofShader 	shader;
		ofFbo 		fbo;	
};

