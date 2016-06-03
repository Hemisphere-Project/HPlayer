#pragma once

#include "ofxOMXPlayer.h"
#include "ofxOMXPlayerListener.h"


/**
 * A datastructure to store data about screen/display dimensions.
 */
struct dims_t {
    int height; /*!< height of the display (screen) */
	int width; /*!< width of the display (screen) */
	int marginX; /*!< horizontal display margin */
	int marginY; /*!< vertical display margin */
};


/**
 * A datastructure to store data about image display.
 */
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
        virtual void onVideoEnd() = 0; /*!< handler for video end \todo reality check */
        virtual void onVideoFreeze() = 0; /*!< handler for frozen video \todo reality check */
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
		void buffer(); /*!< \todo implementation (?) */
		void display(); /*!< \todo implementation (?) */
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

        // FX
		void blur();

		//BUFFER & SHADER
		ofShader blurH;  /*!< shader for horizontal blurring */
        	ofShader blurV;  /*!< shader for vertical blurring */
		ofFbo framebuffer; /*!< plain frambuffer */
        	ofFbo frameblur; /*!< framebuffer for blurring */

		//PARAMS
		dims_t			dim; /*!< screen dimensions */
		params_t		params;	/*!< playback parameters */
		ofxOMXPlayerSettings 	_settings;

		//END DETECT & ANTIFREEZE
		int lastFrame; /*!< last frame displayed */
    		int freeze; /*!< counter for frozen frames (?) \todo reality check */

    		omListener*	listener; /*!< event listener */
};

