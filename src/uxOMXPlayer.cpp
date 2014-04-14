#include "uxOMXPlayer.h"

//--------------------------------------------------------------
void uxOMXPlayer::init(bool glsl)
{			
	//GLSL CONFIG 
	enableGLSL = glsl;
	
	//SCAN VIDEO FOLDER
	currentIndex = 0;
	ofDirectory currentVideoDirectory(ofToDataPath(VIDEO_DIR, true));
	if (currentVideoDirectory.exists()) 
	{
		currentVideoDirectory.listDir();
		videoFiles = currentVideoDirectory.getFiles();
	}

	//SHADER AND FRAME BUFFER LOAD
	if (enableGLSL) 
	{
		shader.load("shader/shaderExample");
		fbo.allocate(ofGetWidth(), ofGetHeight());
	}
	
	//OMXPLAYER Settings
	currentVolume = 0.5;
	muteVolume = false;
	settings.doFlipTexture		= false; //true on older firmware
	settings.useHDMIForAudio 	= true;
	settings.enableTexture 		= true;
	settings.enableLooping 		= true;	
	settings.videoPath 			= "";
	
	ofLog(OF_LOG_NOTICE,"-HP- omxPlayer initialized");
}

//--------------------------------------------------------------
void uxOMXPlayer::display(){
	
	if (!this->isPlaying()) return;
	
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
				this->draw(0,0);
			shader.end();
		fbo.end();
		
		fbo.draw(0, 0);
	}
	else this->draw(0, 0, ofGetWidth(), ofGetHeight());
}


//--------------------------------------------------------------
void uxOMXPlayer::play(){
	if (settings.videoPath != "") this->play(settings.videoPath);
	else this->play(currentIndex);
}

//--------------------------------------------------------------
void uxOMXPlayer::play(int index){

	if ((index >= 0) && (index < videoFiles.size())) 
	{
		currentIndex = index;
		this->play(videoFiles[currentIndex].path());
	}
}

//--------------------------------------------------------------
void uxOMXPlayer::play(string filepath){
	
	ofFile file(filepath);	
	if (!file.isFile()) 
	{
		ofLog(OF_LOG_NOTICE,"-HP- file not found: "+filepath);
		return;
	}
	settings.videoPath = file.path();
		
	if (this->isOpen) this->loadMovie(settings.videoPath);
	else this->setup(settings);
		
	this->volume();	
	this->setPaused(false);
	
	ofLog(OF_LOG_NOTICE,"-HP- play "+settings.videoPath);
}

//--------------------------------------------------------------
void uxOMXPlayer::next(){

	int index = currentIndex+1;
	if (index >= videoFiles.size()) index = 0;
	this->play(index);
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
int uxOMXPlayer::getVolumeInt(){
	return static_cast<int>(currentVolume*100.0);
}

//--------------------------------------------------------------
string uxOMXPlayer::getFile(){
	return settings.videoPath;
}

//--------------------------------------------------------------
int uxOMXPlayer::getPositionMs(){
	return 0;
}

//--------------------------------------------------------------
int uxOMXPlayer::getDurationMs(){
	return 100;
}


