#include "uxOMXPlayer.h"

//--------------------------------------------------------------
void uxOMXPlayer::init(bool audioHDMI, bool autoLoop, bool glsl)
{			
	//GLSL CONFIG 
	enableGLSL = glsl;
	
	//SCAN VIDEO FOLDER
	currentIndex = 0;
	videoFiles.clear();

	//SHADER AND FRAME BUFFER LOAD
	if (enableGLSL) 
	{
		shader.load("shader/shaderExample");
		fbo.allocate(ofGetWidth(), ofGetHeight());
	}
	
	//OMXPLAYER Settings
	currentVolume = 0.5;
	muteVolume = false;
	this->settings.doFlipTexture		= false; //true on older firmware
	this->settings.useHDMIForAudio 		= audioHDMI;
	this->settings.enableTexture 		= true;	
	this->settings.videoPath 			= "";
	this->setLoop(autoLoop);
	
	ofLog(OF_LOG_NOTICE,"-HP- omxPlayer initialized");
}

//--------------------------------------------------------------
void uxOMXPlayer::display(){
	
	if (!this->isPlaying()) return;
	
	//WIDTH
	if (this -> getHeight() > 0) this->dim.width = floor( ofGetHeight() * this->getWidth() / this -> getHeight() );
	else this->dim.width = 0;
	if (this->dim.width > ofGetWidth()) this->dim.width = ofGetWidth();
	
	//HEIGHT
	if (this -> getWidth() > 0) this->dim.height = floor( this->dim.width * this -> getHeight() / this->getWidth() );
	else this->dim.height = 0;
	
	//DRAW IF DIMS ARE VALID
	if ((this->dim.width > 0) && (this->dim.height > 0))
	{
		//MARGINS
		this->dim.marginX = floor((ofGetWidth()-this->dim.width)/2);
		this->dim.marginY = floor((ofGetHeight()-this->dim.height)/2);
	
		//DRAW VIDEO
		if (enableGLSL)
		{
			fbo.begin();
				shader.begin();
					//Here we tell pass our shader some changing values
			
					//We pass our texture id 
					shader.setUniformTexture("tex0", this->getTextureReference(), this->getTextureID());
			
					//We give it an incrementing value to use
					shader.setUniform1f("time", ofGetElapsedTimef());
			
					//And a resolution
					shader.setUniform2f("resolution", ofGetWidth(), ofGetHeight());
	
					//We then send our texture that kicks it off
					this->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
				shader.end();
			fbo.end();
		
			fbo.draw(0, 0);
		}
		else this->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
	}
	else 
	{
		this->dim.marginX = 0;
		this->dim.marginY = 0;
	}
}

/*PLAY FILE LIST*/
//--------------------------------------------------------------
void uxOMXPlayer::play(vector<string> playlist, bool doLoop)
{
	vector<ofFile> list;
	
	for(int k = 0; k < playlist.size(); k++)
	{			
		ofLog(OF_LOG_NOTICE,"-HP- add  "+playlist[k]+" to the playlist");
		ofFile file(playlist[k]);
		if (file.isFile()) list.push_back(file);
		else if (file.isDirectory())
		{
			ofDirectory dir(playlist[k]);
			dir.listDir();
			for(int j = 0; j < dir.size(); j++)
				if (dir.getFile(j).isFile()) list.push_back(dir.getFile(j));
		}
	}
	
	for(int k = 0; k < list.size(); k++)
		ofLog(OF_LOG_NOTICE,"-HP- "+list[k].path()+" in the playlist");
	
	this->videoFiles = list;
	
	//NO FILES IN THE LIST: DO NOTHING
	if (list.size() == 0) 
	{
		this->stop();
		return;
	}
	
	this->setLoop(doLoop);
	
	//START PLAY AT 0
	this->play(0);
}

/*PLAY FILE*/
//--------------------------------------------------------------
void uxOMXPlayer::play(string file, bool doLoop)
{
	vector<string> playlist;
	playlist.push_back(file);
	this->play(playlist,doLoop);
}

/*PLAY FILE AT INDEX IN THE LIST*/
//--------------------------------------------------------------
void uxOMXPlayer::play(int index){
	
	if ((index >= 0) && (index < videoFiles.size())) 
	{
		currentIndex = index;		
		this->settings.videoPath = videoFiles[currentIndex].path();
		this->play();
	}
}

/*PLAY FILE WITH SPECIFIED SETTINGS*/
//--------------------------------------------------------------
void uxOMXPlayer::play(){
		
	if (this->isOpen) this->stop(); 
	this->setup(this->settings);
		
	this->volume();	
	this->setPaused(false);
	
	ofLog(OF_LOG_NOTICE,"-HP- playing "+this->settings.videoPath);
}

/*PLAY NEXT FILE IN THE LIST*/
//--------------------------------------------------------------
void uxOMXPlayer::next()
{
	//if only one file
	if (this->playlistSize() == 1)
	{
		if (!this->isLoop()) this->stop();
		return;
	}
	
	//playlist
	int index = currentIndex+1;
	if (index >= videoFiles.size()) 
	{
		if (this->isLoop()) this->play(0);
		else this->stop();
	}
	else this->play(index);
}

/*PLAY PREVIOUS FILE IN THE LIST*/
//--------------------------------------------------------------
void uxOMXPlayer::prev()
{
	//if only one file
	if (this->playlistSize() == 1)
	{
		if (!this->isLoop()) this->stop();
		return;
	}
	
	int index = currentIndex-1;
	if (index < 0) 
	{
		if (this->isLoop()) this->play((videoFiles.size()-1));
		else this->stop();
	}
	else this->play(index);
}

//--------------------------------------------------------------
void uxOMXPlayer::stop(){
	
	this->close();
	this->getTextureReference().clear();
	
	ofLog(OF_LOG_NOTICE,"-HP- stop ");
}

//--------------------------------------------------------------
void uxOMXPlayer::pause(){
	this->setPaused(true);
	
	ofLog(OF_LOG_NOTICE,"-HP- pause ");
}

//--------------------------------------------------------------
void uxOMXPlayer::resume(){
	this->setPaused(false);
	
	ofLog(OF_LOG_NOTICE,"-HP- resume ");
}

//--------------------------------------------------------------
void uxOMXPlayer::setLoop(bool doLoop){
	
	this->enableLoopingList = false;
	this->settings.enableLooping = false;
	
	//ONLY ONE FILE IN THE LIST
	if (videoFiles.size() == 1)
	{
		if (this->settings.enableLooping != doLoop)
		{
			this->settings.enableLooping = doLoop;
			if (this->isPlaying()) this->play();
		}
	}
	
	//NONE OR MULTIPLE FILES PLAYLIST
	else this->enableLoopingList = doLoop;	
}

//--------------------------------------------------------------
bool uxOMXPlayer::isLoop(){
	return (this->settings.enableLooping || this->enableLoopingList);
}


//--------------------------------------------------------------
void uxOMXPlayer::volume(){
	this->volume(currentVolume);
}

//--------------------------------------------------------------
void uxOMXPlayer::volume(int v){
	this->volume(v/100.0);
}


//--------------------------------------------------------------
void uxOMXPlayer::volume(double v){
	if (v > 1.0) v=v/100.0;
	currentVolume = v;
		
	if (muteVolume) this->setVolume(0.0);
	else this->setVolume(currentVolume);

	ofLog(OF_LOG_NOTICE,"-HP- volume "+ofToString(this->getVolumeInt()));
}

//--------------------------------------------------------------
bool uxOMXPlayer::isMuted(){
	return muteVolume;
}

//--------------------------------------------------------------
void uxOMXPlayer::setMuted(bool mute){
	muteVolume = mute;
	this->volume();
}

//--------------------------------------------------------------
int uxOMXPlayer::playlistSize(){
	return videoFiles.size();
}

//--------------------------------------------------------------
int uxOMXPlayer::getVolumeInt(){
	return static_cast<int>(currentVolume*100.0);
}

//--------------------------------------------------------------
string uxOMXPlayer::getFile(){
	return this->settings.videoPath;
}

//--------------------------------------------------------------
int uxOMXPlayer::getCurrentFrameNbr(){
	return (this->getCurrentFrame() % this->getTotalNumFrames());
}

//--------------------------------------------------------------
int uxOMXPlayer::getPositionMs(){
	return static_cast<int>(this->getCurrentFrameNbr() * this->getDurationMs() / this->getTotalNumFrames());
}

//--------------------------------------------------------------
int uxOMXPlayer::getDurationMs(){
	return static_cast<int>(this->getDuration()*1000);
}


