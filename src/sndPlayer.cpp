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


/**
 * Continue after freezing a frame (?).
 *
 * \see resume()
 * \see run()
 * \see play(string file)
 * \todo reality check for description
 */
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


/**
 * Set the volume according to the internal parameters.
 *
 * \see paramsSND_t
 */
void sndPlayer::applyVolume()
{
    //APPLY VOLUME CHANGES
    float v;
    if (params.mute) v = 0.0;
    else v = params.volume/100.0;
    if (player != NULL) player->setVolume(v);
}


/**
 * Play a given file.
 *
 * \param file file (path) to play
 * \see run()
 * \see stop()
 * \see pause()
 * \see resume()
 */
void sndPlayer::play(string file)
{
    this->stop();
    player = new ofOpenALSoundPlayer();
    player->loadSound( file, true );
    player->play();
}


/**
 * Check if there is a file played currently.
 *
 * \returns true in case of an ongoing playback
 * \see play(string file)
 */
bool sndPlayer::isPlaying()
{
    return (player != NULL and player->getIsPlaying());
}


/**
 * Check if a playback is currently paused.
 *
 * \returns true in case of a pause in an ongoing playback
 */
bool sndPlayer::isPaused()
{
    return (player != NULL and player->getIsPaused());
}

/**
 * Seek to a given timestamp.
 *
 * \param timemilli timestamp in file given in milliseconds
 * \see getPositionMs()
 * \see getDurationMs()
 */
void sndPlayer::seek(int timemilli)
{        
    if (player != NULL) player->setPositionMS(timemilli);
}


/**
 * Stop the player.
 *
 * \see pause()
 * \see play(string file)
 * \see setMuted(bool mute)
 */
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


/**
 * Pause an ongoing playback.
 *
 * \see resume()
 * \see stop()
 * \see setMuted(bool mute)
 * \see play(string file)
 */
void sndPlayer::pause(){
    if (player != NULL) player->setPaused(true);
}


/**
 * Continue an ongoing playback after a pause.
 *
 * \see pause()
 * \see play(string file)
 */
void sndPlayer::resume(){
    if (player != NULL) player->setPaused(false);
}


/**
 * Set volume for playback.
 *
 * \param v volume
 * \see setMuted(bool mute)
 */
void sndPlayer::volume(int v){
    params.volume = v;
}


/**
 * Mute an ongoing playback.
 *
 * \see stop()
 * \see pause()
 * \see volume(int v)
 */
void sndPlayer::setMuted(bool mute){
    params.mute = mute;
}

/**
 * Get current position in miliseconds.
 * 
 * \return current position on miliseconds
 * \see seek(int timemilli)
 * \see getDurationMs()
 */
int sndPlayer::getPositionMs(){
    if (player != NULL) return player->getPositionMS();
    return 0;
}


/**
 * Get the duration of the current playback.
 *
 * \return duration of playback in miliseconds
 * \see getPositionMs()
 * \see seek(int timemilli)
 */
int sndPlayer::getDurationMs(){
    if (player != NULL)
    {
        float pos = player->getPosition();
        if (pos > 0) return static_cast<int>(player->getPositionMS()*100/pos);
    }
    return 0;
}

