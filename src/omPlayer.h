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
    int     volume; /*!< playback volume */
    bool    mute;   /*!< indicate that playback is muted */
    int     blur;   /*!< indicate amount of blur */
    int     zoom;   /*!< inidcate zoom level in percent */
};


/**
 * Event listener (?)
 *
 * \todo reality check
 */
class omListener
{
	public:
		virtual void onVideoEnd() = 0;
		virtual void onVideoFreeze() = 0;
};


/**
 * Class to control the omxplayer
 */
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
		void iceBreak();
		void makeRect();
		void clearscreen();

		//FX
		void blur();

		//BUFFER & SHADER
		ofShader blurH, blurV;
		ofFbo framebuffer, frameblur;

		//PARAMS
		dims_t			dim; /*!< screen dimensions */
		params_t		params;	/*!< playback parameters */

		//END DETECT & ANTIFREEZE
		int lastFrame;
    	int freeze;

    	omListener*	listener; /*!< event listener */
};

