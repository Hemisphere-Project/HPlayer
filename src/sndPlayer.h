#pragma once
#include "ofSoundPlayer.h"

struct paramsSND_t {
    int     volume;
    bool    mute;
};

class sndListener
{
    public:
        virtual void onSoundEnd() = 0;
        virtual void onSoundFreeze() = 0;
};

class sndPlayer 
{
    
    public:

        //RUN
        sndPlayer();
        void run();
        void setListener(sndListener* myListener);
        
        //CONTROL
        void play(string file);
        void stop();
        void pause();
        void resume();
        
        void seek(int timemilli);
        
        void volume(int v);
        void setMuted(bool mute);
        
        //STATE
        bool    isPlaying();
        bool    isPaused();
        int     getPositionMs();
        int     getDurationMs();
        
    private:
            
        //DISPLAY LOGICS
        void applyVolume();  
        void iceBreak(); 
                    
        //PARAMS
        paramsSND_t        params; 

        ofOpenALSoundPlayer* player;

        //EVENTS
        sndListener* listener;

        //END DETECT & ANTIFREEZE
        float lastPos;
        int freeze;
};