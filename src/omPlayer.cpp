#include "omPlayer.h"

omPlayer::omPlayer():ofxOMXPlayer()
{
	listener = NULL;
}

//--------------------------------------------------------------
void omPlayer::init(bool textured, bool audioHDMI)
{	
	//OMXPLAYER Settings
	settings.videoPath 			= "";
	settings.useHDMIForAudio 	= audioHDMI;	
	settings.doFlipTexture		= false; //true on older firmware
	settings.enableTexture 		= textured;	
	settings.initialVolume		= 0.5;
	settings.enableLooping 		= false;
	
	//params DEFAULTS
	params.volume = settings.initialVolume;
	params.mute = false;
	params.blur = 0;
	params.zoom = 100;

	//SHADERS
	noshader.load("NoShader");
	blurV.load("VerticalBlur");
	blurH.load("HorizontalBlur");
	
	//FRAMEBUFFERS
	if (settings.enableTexture)
	{
		framebuffer.allocate(ofGetWidth(), ofGetHeight());
		frameblur.allocate(ofGetWidth(), ofGetHeight());
		this->clearscreen();
	}

	//ANTIFREEZE
	lastFrame = 0;
    freeze = 0;

	//START PLAYER
	this->setup(settings);	
}

void omPlayer::setListener(omListener* myListener)
{
	this->listener = myListener;
}

void omPlayer::applyVolume()
{
	//APPLY VOLUME CHANGES
	float v;
	if (params.mute) v = 0.0;
	else v = params.volume/100.0;
	if (v != settings.initialVolume) 
	{
		settings.initialVolume = v;
		this->setVolume(settings.initialVolume);
	}
}

void omPlayer::iceAxe()
{
	//DETECT END / LOOP since Listener in ofxOMX are broken
	int maxFrame = getTotalNumFrames()-1;
	int currentFrame = getCurrentFrameNbr();	
	if (this->listener != NULL)
	{
		//FILE REACH THE END
		if ((currentFrame == maxFrame) and (lastFrame < maxFrame)) listener->onVideoEnd();
		//FREEZE detection (due to wrong frame counter)
		if ((currentFrame == lastFrame) && (!this->isPaused())) 
		{
			if (freeze++ > 10)  listener->onVideoFreeze();
		}
		else freeze = 0;
	}
	lastFrame = currentFrame;
}

void omPlayer::makeRect()
{
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
}

void omPlayer::show()
{
	//APPLY NEW VOLUME / MUTE
	this->applyVolume();

	//CHECK IF PLAYING
	if (!this->isPlaying()) return;

	//ANTI FREEZE
	this->iceAxe();
	
	//CHECK IF TEXTURE MODE ENABLED AND VALID
	if(!this->isTextureEnabled()) return;
	if (!((this->getHeight() > 0) and (this->getWidth() > 0))) return;

	//CALCULATE DIMS
	this->makeRect();

	//CLEAR BUFFER
	this->clearscreen();

	//FILL BUFFER
	framebuffer.begin();
		this->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
	framebuffer.end();

	//BLUR
	if (params.blur > 0) this->blur();	

	//DRAW TO SCREEN
	framebuffer.draw(0, 0);
}


void omPlayer::clearscreen()
{
	if (!this->isTextureEnabled()) return;
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


/*START PLAYER*/
//--------------------------------------------------------------
void omPlayer::play(string file){
		
	/*if (this->isPlaying()) this->stop(); 
	this->settings.videoPath = file;
	this->setup(this->settings);
	*/
	this->loadMovie(file);
	this->setPaused(false);	
}

/*SEEK TO TIME MILLISECONDS*/
//--------------------------------------------------------------
void omPlayer::seek(int timemilli){
		
	//int frame = this->timeToFrameMs(timemilli);
	//TODO
}


//--------------------------------------------------------------
void omPlayer::stop()
{	
	this->close();
	this->clearscreen();
}

//--------------------------------------------------------------
void omPlayer::pause(){
	this->setPaused(true);
}

//--------------------------------------------------------------
void omPlayer::resume(){
	this->setPaused(false);
}

//--------------------------------------------------------------
void omPlayer::volume(int v){
	params.volume = v;
}

//--------------------------------------------------------------
void omPlayer::setMuted(bool mute){
	params.mute = mute;
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
	return static_cast<int>(this->getDurationInSeconds()*1000);
}

//--------------------------------------------------------------
int omPlayer::timeToFrameMs(int timemilli){
	int frame = static_cast<int>(timemilli * this->getTotalNumFrames() / this->getDurationMs());
	return (frame % this->getTotalNumFrames());
}

