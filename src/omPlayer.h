#pragma once

#include "ofxOMXPlayer.h"
#include "ofxOMXPlayerListener.h"

struct dims_t {
	int height;
	int width;
	int marginX;
	int marginY;
};

struct params_t {
    int     volume;
    bool    mute;
    int     blur;
    int     zoom;
};

class omListener
{
	public:
		virtual void onVideoEnd() = 0;
		virtual void onVideoFreeze() = 0;
};

class omPlayer : public ofxOMXPlayer 
{
	
	public:

		//RUN
		omPlayer();
		void init(bool textured, bool audioHDMI);
		void setListener(omListener* myListener);

		void buffer();
		void display();
		void show();
		
		//CONTROL
		void play(string file);
		void stop();
		void pause();
		void resume();
		
		void seek(int timemilli);
		
		void volume(int v);
		void setMuted(bool mute);
		void setBlur(int blur);
		void setZoom(int zoom);
		
		//STATE
		int 	getCurrentFrameNbr();
		int 	getPositionMs();
		int 	getDurationMs();
		int 	timeToFrameMs(int timemilli);
		
	private:
			
		//DISPLAY LOGICS
		void applyVolume();
		void iceAxe();
		void makeRect();
		void clearscreen();
		
		//FX
		void blur();
		
		//BUFFER & SHADER
		ofShader noshader, blurH, blurV;
		ofFbo framebuffer, frameblur;				
					
		//PARAMS
		dims_t			dim;
		params_t		params;	

		//END DETECT & ANTIFREEZE
		int lastFrame;
    	int freeze;

    	//EVENTS
    	omListener*	listener;
};

