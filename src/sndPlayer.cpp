#include "sndPlayer.h"

/**
 * Constructor for the `sndPlayer` class to set default values.
 */
sndPlayer::sndPlayer() {
    player = NULL;
    params.volume = 50;
    params.mute = false;

    this->lastPos = 0.0;
    this->freeze = 0;
    this->listener = NULL;
}

void sndPlayer::setListener(sndListener* myListener)
{
    this->listener = myListener;
}

void sndPlayer::run()
{
    this->applyVolume();
    this->iceBreak();
}

void sndPlayer::iceBreak()
{
    if (player != NULL)
    {
        float currentPos = player->getPosition();

        //DETECT END / LOOP
        if (this->listener != NULL)
        {
            //FILE REACH THE END
            if (lastPos > currentPos) listener->onSoundEnd();

            //FREEZE detection (due to wrong frame counter)
            if ((lastPos == currentPos) && (!this->isPaused())) 
            {
                if (freeze++ > 10)  listener->onSoundFreeze();
            }
            else freeze = 0;
        }
        lastPos = currentPos;
    }
}

void sndPlayer::applyVolume()
{
    //APPLY VOLUME CHANGES
    float v;
    if (params.mute) v = 0.0;
    else v = params.volume/100.0;
    if (player != NULL) player->setVolume(v);
}

//--------------------------------------------------------------
void sndPlayer::play(string file)
{
    this->stop();
    player = new ofOpenALSoundPlayer();
    player->loadSound( file, true );
    player->play();
}

bool sndPlayer::isPlaying()
{
    return (player != NULL and player->getIsPlaying());
}

bool sndPlayer::isPaused()
{
    return (player != NULL and player->getIsPaused());
}

/*SEEK TO TIME MILLISECONDS*/
//--------------------------------------------------------------
void sndPlayer::seek(int timemilli)
{        
    if (player != NULL) player->setPositionMS(timemilli);
}


//--------------------------------------------------------------
void sndPlayer::stop()
{   
    if (player != NULL)
    {
        player->stop();
        player->close();
        delete player;
        player = NULL;
        lastPos = 0.0;
    }
}

//--------------------------------------------------------------
void sndPlayer::pause(){
    if (player != NULL) player->setPaused(true);
}

//--------------------------------------------------------------
void sndPlayer::resume(){
    if (player != NULL) player->setPaused(false);
}

//--------------------------------------------------------------
void sndPlayer::volume(int v){
    params.volume = v;
}

//--------------------------------------------------------------
void sndPlayer::setMuted(bool mute){
    params.mute = mute;
}

//--------------------------------------------------------------
int sndPlayer::getPositionMs(){
    if (player != NULL) return player->getPositionMS();
    return 0;
}

//--------------------------------------------------------------
int sndPlayer::getDurationMs(){
    if (player != NULL)
    {
        float pos = player->getPosition();
        if (pos > 0) return static_cast<int>(player->getPositionMS()*100/pos);
    }
    return 0;
}

