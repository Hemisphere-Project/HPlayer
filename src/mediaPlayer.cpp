#include "mediaPlayer.h"

/**
 * Constructor to set default values
 *
 * \see init()
 */
mediaPlayer::mediaPlayer()
{
    //GENERIC PARAMS
    this->name    = "HPlayer";
    this->info    = false;
    this->volume  = 50;
    this->mute    = false;
    this->ahdmi   = false;
    this->loop    = true;
    this->random  = false;

    //VIDEO PARAMS
    this->textured = false;
    this->zoom    = 100;
    this->blur    = 0;

    //MEDIA
    this->basePath = "/home/pi/media";
    this->currentIndex = 0;
    this->nextIndex = 0;

    //SOUND
    sound = new sndPlayer();

    //VIDEO
    video = new omPlayer();

    //IMAGE
    image = new imgPlayer();
}

/**
 * Apply settings from XML config file and CMD Line arguments
 *
 */
void mediaPlayer::configure(string file, bool cmdLineOverwrite)
{
  // Retrieve config values from settings file
  if (!file.empty()) {
    xmlSettings settings(file);
    this->name = settings.conf_str("system","playerName",this->name);
  	this->info = settings.conf_bool("system","enableInfo",this->info);
  	this->volume = settings.conf_int("player","volume",this->volume);
  	this->ahdmi = settings.conf_bool("player","audioHdmi",this->ahdmi);
    this->loop = settings.conf_bool("media","loop",this->loop);
    this->random = settings.conf_bool("media","random",this->random);

  	this->textured = settings.conf_bool("player","textured",this->textured);
  	this->zoom = settings.conf_int("player","zoom",this->zoom);
  	this->blur = settings.conf_int("player","blur",this->blur);

  	this->basepath( settings.conf_str("media","path",this->basePath) );
  }

  // Retrieve config values from command line arguments
  if (cmdLineOverwrite) {
    if (ofxArgParser::hasKey("name")) this->name = ofxArgParser::getValue("name");
  	if (ofxArgParser::hasKey("info")) this->info = (ofToInt(ofxArgParser::getValue("info")) == 1);
  	if (ofxArgParser::hasKey("volume")) this->volume = ofToInt(ofxArgParser::getValue("volume"));
  	if (ofxArgParser::hasKey("ahdmi")) this->ahdmi = (ofToInt(ofxArgParser::getValue("ahdmi")) == 1);
    if (ofxArgParser::hasKey("loop")) this->loop = (ofToInt(ofxArgParser::getValue("loop")) == 1);
  	if (ofxArgParser::hasKey("ran")) this->random = (ofToInt(ofxArgParser::getValue("ran")) == 1);

  	if (ofxArgParser::hasKey("gl")) this->textured = (ofToInt(ofxArgParser::getValue("gl")) == 1);
    if (ofxArgParser::hasKey("zoom")) this->zoom = ofToInt(ofxArgParser::getValue("zoom"));
  	if (ofxArgParser::hasKey("blur")) this->blur = ofToInt(ofxArgParser::getValue("blur"));

    if (ofxArgParser::hasKey("path")) this->basepath( ofxArgParser::getValue("path") );
  }
}

/**
 * Initalize all sound, video and image handling.
 *
 * \see mediaPlayer()
 */
void mediaPlayer::init()
{
    //VIDEO
    video->init(textured, ahdmi);
    video->setListener(this);

    //SOUND
    sound->setListener(this);

    //IMAGE
    image->init(textured);
}


/**
 * Set basic path/directory to load files from.
 *
 * \param path to set
 */
void mediaPlayer::basepath(string path)
{
    ofDirectory dir(path);
    if (dir.isDirectory()) basePath = path;
}


/**
 * Update the internal state of the mediaPlayer according to all
 * (changed) parameters. This method is to be used in conjunction with
 * openFrameworks.
 */
void mediaPlayer::update()
{
    //CHANGE MEDIA
    if (this->nextIndex != this->currentIndex)
    {
        if (this->nextIndex >= this->playlistSize() && this->loop) this->nextIndex = 0;
        else if (this->nextIndex < 0 && this->loop) this->nextIndex = (this->playlistSize()-1);
        this->play(this->nextIndex);
        this->nextIndex = this->currentIndex;
    }

    //VIDEO
    video->volume(this->volume);
    video->setMuted(this->mute);
    video->setZoom(this->zoom);
    video->setBlur(this->blur);

    //IMAGE
    image->setZoom(this->zoom);
    image->setBlur(this->blur);

    //SOUND
    sound->volume(this->volume);
    sound->setMuted(this->mute);
    sound->run();
}

/**
 * Output visual data on screen. To be used with openFrameworks.
 *
 * \see clearscreen()
 */
void mediaPlayer::draw()
{
    //VIDEO
    video->show();

    //IMAGE
    image->show();

    //MESSAGE
    if (!this->isPlaying()) this->displayStandby();
    if (info) this->displayInfo();
}


/**
 * Load files into playlist based on the base path.
 *
 * \see load()
 * \see play(string file)
 * \see play()
 * \see play(int index)
 * \see play(vector<string> playlist)
 */
void mediaPlayer::load(vector<string> playlist)
{
    vector<ofFile> list;
    for(int k = 0; k < playlist.size(); k++)
    {
        ofFile file(playlist[k]);
        if (file.isFile()) list.push_back(file);
        else if (file.isDirectory())
        {
            ofDirectory dir(playlist[k]);
            dir.sort();
            dir.listDir();
            for(int j = 0; j < dir.size(); j++)
                if (dir.getFile(j).isFile()) list.push_back(dir.getFile(j));
        }
        else
        {
            ofFile relfile(basePath+playlist[k]);
            if (relfile.isFile()) list.push_back(relfile);
        }
    }
    this->mediaFiles = list;
}


/**
 * Load the internal playlist based on the base path.
 *
 * \see load(vector<string> playlist)
 * \see play(string file)
 * \see play()
 * \see play(int index)
 * \see play(vector<string> playlist)
 */
void mediaPlayer::load()
{
    vector<string> playlist;
    playlist.push_back(basePath);
    this->load(playlist);
}


/**
 * Get size (length) of playlist
 *
 * \returns number of items in playlist
 */
int mediaPlayer::playlistSize(){
    return mediaFiles.size();
}


/**
 * Play a files from a given list.
 *
 * \param playlist with filenames to play
 * \see play(string file)
 * \see play()
 * \see play(int index)
 * \see stop()
 * \see prev()
 * \see next()
 * \see isPlaying()
 */
void mediaPlayer::play(vector<string> playlist)
{
    this->load(playlist);
    this->play();
}


/**
 * Play a single given file.
 *
 * \param file to play
 * \see play(vector<string> playlist)
 * \see play()
 * \see play(int index)
 * \see stop()
 * \see prev()
 * \see next()
 * \see isPlaying()
 */
void mediaPlayer::play(string file)
{
    vector<string> playlist;
    playlist.push_back(file);
    this->play(playlist);
}


/**
 * Play the first item of the (internal) playlist.
 *
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play(int index)
 * \see stop()
 * \see prev()
 * \see next()
 * \see isPlaying()
 */
void mediaPlayer::play(){
    if (this->playlistSize() > 0) this->play(0);
    else this->stop();
}


/**
 * Play specific item of the (internal) playlist.
 *
 * \param index of file in playlist to play
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play()
 * \see stop()
 * \see prev()
 * \see next()
 * \see isPlaying()
 */
void mediaPlayer::play(int index)
{
    if ((index >= 0) && (index < this->playlistSize()))
    {
        this->currentIndex = index;

        ofFile file = mediaFiles[this->currentIndex];
        string ext = ofToLower( file.getExtension() );

        //VIDEO
        if (ext == "mp4" or ext == "mov" or ext == "avi" or ext == "m4v")
        {
            sound->stop();
            image->stop();
            video->play( file.path() );
        }
        //SOUND
        else if (ext == "wav" or ext == "mp3" or ext == "aif" or ext == "ogg")
        {
            video->stop();
            image->stop();
            sound->play( file.path() );
        }
        //IMAGE
        else if (ext == "bmp" or ext == "jpg" or ext == "jpeg" or ext == "gif" or ext == "tiff" or ext == "png")
        {
            video->stop();
            sound->stop();
            image->play( file.path() );
        }
    }
    else  this->stop();
}


/**
 * Play next file in playlist.
 *
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play(int index)
 * \see play()
 * \see stop()
 * \see prev()
 */
void mediaPlayer::next()
{
    if (this->random) this->nextIndex = (rand() % (int)( this->playlistSize() ));
    else this->nextIndex = this->currentIndex+1;
    //ofLog(OF_LOG_NOTICE, "Random: " + ofToString(this->nextIndex) + " / " + ofToString(this->playlistSize()-1) );
}


/**
 * Handle end of video file.
 *
 * \see onVideoFreeze()
 * \see onSoundEnd()
 * \see onSoundFreeze()
 */
void mediaPlayer::onVideoEnd()
{
    this->next();
}


/**
 * Handle freezing video.
 *
 * \see onVideoEnd()
 * \see onSoundEnd()
 * \see onSoundFreeze()
 */
void mediaPlayer::onVideoFreeze()
{
    this->next();
}


/**
 * Handle end of video file.
 *
 * \see onSoundFreeze()
 * \see onVideoEnd()
 * \see onVideoFreeze()
 */
void mediaPlayer::onSoundEnd()
{
    this->next();
}


/**
 * Handle freezing sound.
 *
 * \see onSoundEnd()
 * \see onVideoEnd()
 * \see onVideoFreeze()
 */
void mediaPlayer::onSoundFreeze()
{
    this->next();
}


/**
 * Play previous file in playlist.
 *
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play(int index)
 * \see play()
 * \see stop()
 * \see next()
 */
void mediaPlayer::prev()
{
    this->nextIndex = this->currentIndex-1;
}


/**
 * Stop current playback.
 *
 * \see play()
 * \see pause()
 * \see resume()
 * \see next()
 * \see prev()
 */
void mediaPlayer::stop()
{
    //VIDEO & SOUND
    video->stop();
    sound->stop();
    image->stop();
    this->currentIndex = 0;
    this->nextIndex = 0;
}

/**
 * Pause current video and sound playback.
 *
 * \see play()
 * \see stop()
 * \see resume()
 * \see next()
 * \see prev()
 * \see displayStandby()
 */
void mediaPlayer::pause(){
    //VIDEO & SOUND
    video->pause();
    sound->pause();
}


/**
 * Resume currently paused video and sound playback.
 *
 * \see play()
 * \see stop()
 * \see pause()
 * \see next()
 * \see prev()
 */
void mediaPlayer::resume(){
    //VIDEO & SOUND
    video->resume();
    sound->resume();
}


/**
 * Seek to position inidated by the timestamp
 *
 * \param timemilli time in milliseconds
 * \see getPositionMs()
 * \see getDurationMs()
 */
void mediaPlayer::seek(int timemilli){
    //VIDEO & SOUND
    video->seek(timemilli);
    sound->seek(timemilli);
}


/**
 * Check if there is currently a playback going on.
 *
 * \returns indication of playback
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play(int index)
 * \see play()
 * \see isPaused()
 */
bool mediaPlayer::isPlaying()
{
    //VIDEO & SOUND
    return (video->isPlaying() or sound->isPlaying() or image->isPlaying());
}


/**
 * Check if there is currently a paused playback.
 *
 * \returns indication of paused playback
 * \see isPlaying()
 * \see play(vector<string> playlist)
 * \see play(string file)
 * \see play(int index)
 * \see play()
 */
bool mediaPlayer::isPaused()
{
    //VIDEO & SOUND
    if (video->isPlaying()) return video->isPaused();
    else if (sound->isPlaying()) return sound->isPaused();
    return false;
}


/**
 * Get info (path?) on currently playing media.
 *
 * \returns path media file
 * \see displayInfo()
 * \todo reality check
 */
string mediaPlayer::media()
{
    if (this->isPlaying()) return mediaFiles[this->currentIndex].path();
    return "";
}


/**
 * Get current playback position.
 *
 * \returns playback position in milliseconds
 * \see seek(int timemilli)
 * \see getDurationMs()
 */
int mediaPlayer::getPositionMs()
{
    if (video->isPlaying()) return video->getPositionMs();
    else if (sound->isPlaying()) return sound->getPositionMs();
    return 0;
}

/**
 * Get playback duration.
 *
 * \returns duration in milliseconds
 * \see seek(int timemilli)
 * \see getPositionMs()
 */
int mediaPlayer::getDurationMs()
{
    if (video->isPlaying()) return video->getDurationMs();
    else if (sound->isPlaying()) return sound->getDurationMs();
    return 0;
}


/**
 * Display info about media and playback (filenames, framerate, playback
 * status etc.).
 *
 * \see displayStandby()
 */
void mediaPlayer::displayInfo() {
    stringstream info;
    info << "- HPLAYER INFO -\n";

    info <<"\n" << name;
    info <<"\n" << ofToString(ofGetFrameRate()) + "FPS";

    info <<"\n";
    if (this->isPlaying()) info << "PLAYING" ;
    else  info << "STOPPED";
    if (this->isPaused()) info<<"\n" << " - PAUSED";
    info << "\n";

    info <<"\n" <<  "VOL " << volume;
    info <<"\n";
    info <<"\n" <<  ofGetWidth()<<"x"<<ofGetHeight();

    if (video->isPlaying())
    {
        info <<"\n" <<  video->getWidth()<<"x"<< video->getHeight();
        info <<"\n" <<  video->getCurrentFrameNbr() << " / " << video->getTotalNumFrames();
    }

    if (this->isPlaying())
        info <<"\n" <<  ofToString(this->getPositionMs()) << " / " << ofToString(this->getDurationMs());

    info <<"\n";
    //info <<"\n" << osc.log();
    ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}


/**
 * Display media info on standby.
 *
 * \see displayInfo()
 * \see play()
 * \see stop()
 */
void mediaPlayer::displayStandby() {
    if (!name.empty() && name.compare("no") != 0) {
      stringstream info;
      info << " .:: "<<name<<" ::. ";
      ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
    }
}
