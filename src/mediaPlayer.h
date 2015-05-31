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
        string  name;
        bool    info;
        int     volume;
        bool    mute;
        bool    ahdmi;
        bool    loop;
        bool    random;

        //VIDEO PARAMS
        bool    textured;
        int     zoom; /*!< zoom factor in percent */
        int     blur; /*!< amount of blurring (effect) in video */

        
    private:

        void clearscreen();
        sndPlayer* sound; /*!< handle audion content */
        omPlayer* video; /*!< handle video content */
        imgPlayer* image; /*!< handle image content */
        string  basePath; /*!< base directory to load files from */
        vector<ofFile>  mediaFiles; /*!< files to play */
        int             currentIndex; /*!< current index (in playlist) being played */
        int             nextIndex; /*!< next index (in playlist) to play */
};
