#include "omPlayer.h"

//--------------------------------------------------------------
void omPlayer::init(bool audioHDMI)
{	
	//OMXPLAYER Settings
	settings.videoPath 			= "";
	settings.useHDMIForAudio 	= audioHDMI;	
	settings.doFlipTexture		= false; //true on older firmware
	settings.enableTexture 		= true;	
	settings.initialVolume		= 0.5;
	
	//params DEFAULTS
	params.name = "Raymond";
	params.volume = settings.initialVolume;
	params.mute = false;
	params.blur = 0;
	params.zoom = 100;

	//SHADERS
	noshader.load("NoShader");
	blurV.load("VerticalBlur");
	blurH.load("HorizontalBlur");
	
	//FRAMEBUFFERS
	framebuffer.allocate(ofGetWidth(), ofGetHeight());
	frameblur.allocate(ofGetWidth(), ofGetHeight());
	this->clearscreen();

	//START PLAYER
	this->setup(settings);	
}

void omPlayer::buffer()
{	
	//IF NOT PLAYING // DO NOTHING
	if (!this->isPlaying()) return;

	//APPLY VOLUME CHANGES
	this->applyVolume();
	
	//IF NO VIDEO FLUX // DO NOTHING
	if (!this->isTextureEnabled) return;
	if (!((this->getHeight() > 0) and (this->getWidth() > 0))) return;
	
	//WIDTH
	dim.width = floor( ofGetHeight() * this->getWidth() / this->getHeight() ); //WIDTH RATIO FROM MAXIMIZED HEIGHT
	if (dim.width > ofGetWidth()) dim.width = ofGetWidth(); //SHRINK IF WIDTH IS TO BIG
	
	//ZOOM
	if (params.zoom != 100)
	{
		dim.width = floor( dim.width * params.zoom / 100. ); //APPLY ZOOM
		if (dim.width > ofGetWidth()) dim.width = ofGetWidth(); //SHRINK IF WIDTH IS TO BIG
	}
	if (!(dim.width > 0)) return;
	
	//HEIGHT 
	dim.height = floor( dim.width * this->getHeight() / this->getWidth() ); //KEEP ASPECT RATIO
	if (!(dim.height > 0)) return;
	
	//MARGINS
	dim.marginX = floor((ofGetWidth()-dim.width)/2);
	dim.marginY = floor((ofGetHeight()-dim.height)/2);
	
	//CLEAR BUFFER
	this->clearscreen();
	
	//RENDER TO FRAMEBUFFER (WITH BYPASSED SHADER)
	framebuffer.begin();
		noshader.begin();
			noshader.setUniformTexture("tex0", this->getTextureReference(), this->getTextureID());
			this->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
		noshader.end();		
	framebuffer.end();

	//BLUR
	if (params.blur > 0) this->blur();	
}

void omPlayer::display()
{
	if (this->isTextureEnabled) framebuffer.draw(0, 0);
}

void omPlayer::clearscreen()
{
	framebuffer.begin();
		ofClear(0,0,0,0);
	framebuffer.end();
}

void omPlayer::blur()
{	
	for(int i = 0; i < 1; i++) 
	{	
		//BLUR HORIZONTAL
		frameblur.begin();	
			ofClear(0,0,0,0);
			blurH.begin();
				blurH.setUniformTexture("tex0", framebuffer.getTextureReference(0), 0);
				blurH.setUniform1f("radius", (params.blur/2500.));
				framebuffer.draw(0,0);
			blurH.end();
		frameblur.end();
			
		//BLUR VERTICAL
		framebuffer.begin();
			blurV.begin();
				blurV.setUniformTexture("tex0", frameblur.getTextureReference(0), 0);
				blurV.setUniform1f("radius", (params.blur/2500.));
				frameblur.draw(0,0);
			blurV.end();
		framebuffer.end();
	}
}


//CONTROL

/*PLAY FILE LIST*/
//--------------------------------------------------------------
void omPlayer::play(vector<string> playlist, bool doLoop)
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
	}
	
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
void omPlayer::play(string file, bool doLoop)
{
	vector<string> playlist;
	playlist.push_back(file);
	this->play(playlist,doLoop);
}

/*PLAY FILE AT INDEX IN THE LIST*/
//--------------------------------------------------------------
void omPlayer::play(int index){
	
	if ((index >= 0) && (index < videoFiles.size())) 
	{
		currentIndex = index;		
		this->settings.videoPath = videoFiles[currentIndex].path();
		this->play();
	}
}

/*PLAY FILE WITH SPECIFIED SETTINGS*/
//--------------------------------------------------------------
void omPlayer::play(){
		
	if (this->isOpen) this->stop(); 
	this->setup(this->settings);
	
	ofLog(OF_LOG_NOTICE,"-HP- play "+getFile());
			
	this->setPaused(false);
}

/*PLAY NEXT FILE IN THE LIST*/
//--------------------------------------------------------------
void omPlayer::next()
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
void omPlayer::prev()
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
void omPlayer::stop(){
	
	this->close();
	this->clearscreen();
	
	ofLog(OF_LOG_NOTICE,"-HP- stop ");
}

//--------------------------------------------------------------
void omPlayer::pause(){
	this->setPaused(true);
	
	ofLog(OF_LOG_NOTICE,"-HP- pause ");
}

//--------------------------------------------------------------
void omPlayer::resume(){
	this->setPaused(false);
	
	ofLog(OF_LOG_NOTICE,"-HP- resume ");
}

//--------------------------------------------------------------
void omPlayer::setName(string name){
	this->params.name = name;
}

//--------------------------------------------------------------
void omPlayer::volume(int v){
	params.volume = v;
}

//--------------------------------------------------------------
void omPlayer::applyVolume(){
	float v;
	if (params.mute) v = 0.0;
	else v = params.volume/100.0;
	
	if (v != settings.initialVolume) 
	{
		settings.initialVolume = v;
		this->setVolume(settings.initialVolume);
	}
}

//--------------------------------------------------------------
void omPlayer::setMuted(bool mute){
	params.mute = mute;
}

//--------------------------------------------------------------
void omPlayer::setLoop(bool doLoop){
	
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
void omPlayer::setBlur(int blur)
{
	this->params.blur = blur;
}

//--------------------------------------------------------------
void omPlayer::setZoom(int zoom)
{
	this->params.zoom = zoom;
}


//STATE

//--------------------------------------------------------------
string omPlayer::getName(){
	return this->params.name;
}

//--------------------------------------------------------------
int omPlayer::getVolumeInt(){
	return this->params.volume;
}

int omPlayer::getZoom(){
	return this->params.zoom;
}

int omPlayer::getBlur(){
	return this->params.blur;
}

//--------------------------------------------------------------
bool omPlayer::isMuted(){
	return params.mute;
}

//--------------------------------------------------------------
bool omPlayer::isLoop(){
	return (this->settings.enableLooping || this->enableLoopingList);
}

//--------------------------------------------------------------
int omPlayer::getCurrentFrameNbr(){
	return (this->getCurrentFrame() % this->getTotalNumFrames());
}

//--------------------------------------------------------------
int omPlayer::getPositionMs(){
	return static_cast<int>(this->getCurrentFrameNbr() * this->getDurationMs() / this->getTotalNumFrames());
}

//--------------------------------------------------------------
int omPlayer::getDurationMs(){
	return static_cast<int>(this->getDuration()*1000);
}

//--------------------------------------------------------------
int omPlayer::playlistSize(){
	return videoFiles.size();
}

//--------------------------------------------------------------
string omPlayer::getFile(){
	return this->settings.videoPath;
}

