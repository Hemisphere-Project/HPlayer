#include "mediaPlayer.h"

mediaPlayer::mediaPlayer()
{
    //GENERIC PARAMS
    this->name    = "Raymond";
    this->info    = false;
    this->volume  = 50;
    this->mute    = false;
    this->ahdmi   = false;
    this->loop    = true;
    
    //VIDEO PARAMS
    this->textured = false;
    this->zoom    = 100;
    this->blur    = 0;

    video = new omPlayer();
}

//-------------------------------------------------------------
void mediaPlayer::init()
{
    //VIDEO
    video->init(textured, ahdmi);
    video->setListener(this);
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
    //VIDEO
    video->volume(this->volume);
    video->setMuted(this->mute);
    video->setZoom(this->zoom);
    video->setBlur(this->blur);

    video->buffer();
}

//-------------------------------------------------------------
void mediaPlayer::draw()
{
    //VIDEO
    video->display();

    //MESSAGE
    if (!isPlaying()) this->displayStandby();
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

    //NO FILES IN THE LIST: DO NOTHING
    if (this->playlistSize() == 0) 
    {
        this->stop();
        return;
    }
    
    //START PLAY AT 0
    this->play(0);
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
        
    if (playlistSize() > 0) this->play(0);
}

/*PLAY FILE AT INDEX IN THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::play(int index)
{
    if ((index >= 0) && (index < this->playlistSize())) 
    {
        this->currentIndex = index;
    
        //VIDEO
        video->play( mediaFiles[this->currentIndex].path());
    }
}


/*PLAY NEXT FILE IN THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::next()
{    
    //playlist
    int index = this->currentIndex+1;
    if (index >= this->playlistSize()) 
    {
        if (this->loop) this->play(0);
        else this->stop();
    }
    else this->play(index);
}

void mediaPlayer::onVideoEnd()
{
    this->next();
}

void mediaPlayer::onVideoFreeze()
{
    this->next();
}


/*PLAY PREVIOUS FILE IN THE LIST*/
//--------------------------------------------------------------
void mediaPlayer::prev()
{    
    int index = this->currentIndex-1;
    if (index < 0) 
    {
        if (this->loop) this->play((this->playlistSize()-1));
        else this->stop();
    }
    else this->play(index);
}


//--------------------------------------------------------------
void mediaPlayer::stop()
{
    //VIDEO    
    video->stop();
}

//--------------------------------------------------------------
void mediaPlayer::pause(){
    
    //VIDEO
    video->pause();
}

//--------------------------------------------------------------
void mediaPlayer::resume(){
    
    //VIDEO
    video->resume();
}

/*SEEK TO TIME MILLISECONDS*/
//--------------------------------------------------------------
void mediaPlayer::seek(int timemilli){
        
    //int frame = this->timeToFrameMs(timemilli);
    //TODO
}

//--------------------------------------------------------------
bool mediaPlayer::isPlaying()
{
    //VIDEO
    return video->isPlaying();
}

//--------------------------------------------------------------
bool mediaPlayer::isPaused()
{
    //VIDEO
    if (video->isPlaying()) return video->isPaused();
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
    //VIDEO
    if (video->isPlaying()) return video->getPositionMs();
    return 0;
}

//--------------------------------------------------------------
int mediaPlayer::getDurationMs()
{
    //VIDEO
    if (video->isPlaying()) return video->getDurationMs();
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
        info <<"\n" <<  video->getWidth()<<"x"<<video->getHeight();
        info <<"\n" <<  ofToString(video->getPositionMs()) << " / " << ofToString(video->getDurationMs());  
        info <<"\n" <<  video->getCurrentFrameNbr() << " / " << video->getTotalNumFrames(); 
    }
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