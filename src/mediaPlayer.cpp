#include "mediaPlayer.h"

/**
 * Constructor to set default values
 *
 * \see init()
 */
mediaPlayer::mediaPlayer()
{
    //GENERIC PARAMS
    this->name    = "Raymond";
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
    this->currentIndex = 0;
    this->nextIndex = 0;

    //SOUND
    sound = new sndPlayer();

    //VIDEO
    video = new omPlayer();

    //IMAGE
    image = new imgPlayer();
}

//-------------------------------------------------------------
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

//-------------------------------------------------------------
void mediaPlayer::basepath(string path)
{
    ofDirectory dir(path);
    if (dir.isDirectory()) basePath = path;
}

//-------------------------------------------------------------
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

//-------------------------------------------------------------
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

//CONTROL

/*LOAD FILE LIST*/
//--------------------------------------------------------------
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

void mediaPlayer::load()
{
    vector<string> playlist;        
    playlist.push_back(basePath);
    this->load(playlist);
}

//--------------------------------------------------------------
int mediaPlayer::playlistSize(){
    return mediaFiles.size();
}

/*PLAY FILE LIST*/
//--------------------------------------------------------------
void mediaPlayer::play(vector<string> playlist)
{   
    this->load(playlist);
    if (this->playlistSize() > 0) this->play(0);
    else this->stop();
}

/*PLAY FILE*/
//--------------------------------------------------------------
void mediaPlayer::play(string file)
{
    vector<string> playlist;
    playlist.push_back(file);
    this->play(playlist);
}

/*PLAY FIRST OF THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::play(){
        
    if (this->playlistSize() > 0) this->play(0);
}

/*PLAY FILE AT INDEX IN THE LIST*/
//--------------------------------------------------------------
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


/*PLAY NEXT FILE IN THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::next()
{    
    if (this->random) this->nextIndex = (rand() % (int)( this->playlistSize() ));
    else this->nextIndex = this->currentIndex+1;
    //ofLog(OF_LOG_NOTICE, "Random: " + ofToString(this->nextIndex) + " / " + ofToString(this->playlistSize()-1) );
}

void mediaPlayer::onVideoEnd()
{
    this->next();
}

void mediaPlayer::onVideoFreeze()
{
    this->next();
}

void mediaPlayer::onSoundEnd()
{
    this->next();
}

void mediaPlayer::onSoundFreeze()
{
    this->next();
}


/*PLAY PREVIOUS FILE IN THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::prev()
{    
    this->nextIndex = this->currentIndex-1;
}


//--------------------------------------------------------------
void mediaPlayer::stop()
{
    //VIDEO & SOUND    
    video->stop();
    sound->stop();
    image->stop();
    this->currentIndex = 0;
    this->nextIndex = 0;
}

//--------------------------------------------------------------
void mediaPlayer::pause(){
    
    //VIDEO & SOUND
    video->pause();
    sound->pause();
}

//--------------------------------------------------------------
void mediaPlayer::resume(){
    
    //VIDEO & SOUND
    video->resume();
    sound->resume();
}

/*SEEK TO TIME MILLISECONDS*/
//--------------------------------------------------------------
void mediaPlayer::seek(int timemilli){
        
    //VIDEO & SOUND
    video->seek(timemilli);
    sound->seek(timemilli);
}

//--------------------------------------------------------------
bool mediaPlayer::isPlaying()
{
    //VIDEO & SOUND
    return (video->isPlaying() or sound->isPlaying() or image->isPlaying());
}

//--------------------------------------------------------------
bool mediaPlayer::isPaused()
{
    //VIDEO & SOUND
    if (video->isPlaying()) return video->isPaused();
    else if (sound->isPlaying()) return sound->isPaused();
    return false;
}

//--------------------------------------------------------------
string mediaPlayer::media()
{
    if (this->isPlaying()) return mediaFiles[this->currentIndex].path();
    return "";
}

//--------------------------------------------------------------
int mediaPlayer::getPositionMs()
{
    if (video->isPlaying()) return video->getPositionMs();
    else if (sound->isPlaying()) return sound->getPositionMs();
    return 0;
}

//--------------------------------------------------------------
int mediaPlayer::getDurationMs()
{
    if (video->isPlaying()) return video->getDurationMs();
    else if (sound->isPlaying()) return sound->getDurationMs();
    return 0;
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void mediaPlayer::displayStandby() {

    stringstream info;
    info << " .:: "<<name<<" ::. ";
    ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
    
}
