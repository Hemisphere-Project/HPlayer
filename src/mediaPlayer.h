#pragma once
#include "omPlayer.h"
#include "sndPlayer.h"
#include "imgPlayer.h"

class mediaPlayer : public omListener, public sndListener 
{

    public:

        //INIT
        mediaPlayer();
        void init();
        void basepath(string path);

        //RUN
        void update();
        void draw();

        //INFO
        void displayInfo();
        void displayStandby();

        //PLAYBACK
        bool isPlaying();
        bool isPaused();
        int getPositionMs();
        int getDurationMs();

        //CONTROL
        void load(vector<string> playlist);
        void load();
        int  playlistSize();

        void play(vector<string> playlist);
        void play(string file);
        void play(int index);       
        void play();

        void next();
        void prev();
        void stop();
        void pause();
        void resume();
        void seek(int timemilli);

        //EVENTS LISTENER
        void onVideoEnd();
        void onVideoFreeze();
        void onSoundEnd();
        void onSoundFreeze();

        //INFO
        string media();

        //GENERIC PARAMS
        string  name; /*!< name of release \todo reality check */
        bool    info; /*!< control display of (meta) information */
        int     volume; /*!< volume level */
        bool    mute; /*!< indicate mute state */
        bool    ahdmi; /*!< use HDMI for audio output */
        bool    loop;  /*!< loop the current playback */
        bool    random; /*!< indicate random playback of playlist */

        //VIDEO PARAMS
        bool    textured; /*!< indicate the application of video effects */
        int     zoom; /*!< zoom factor in percent */
        int     blur; /*!< amount of blurring (effect) in video */

        
    private:

        void clearscreen(); /*!< \todo implementation (?) */
        sndPlayer* sound; /*!< handle audion content */
        omPlayer* video; /*!< handle video content */
        imgPlayer* image; /*!< handle image content */
        string  basePath; /*!< base directory to load files from */
        vector<ofFile>  mediaFiles; /*!< files to play */
        int             currentIndex; /*!< current index (in playlist) being played */
        int             nextIndex; /*!< next index (in playlist) to play */
};
