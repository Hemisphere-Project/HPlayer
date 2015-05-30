#include "omPlayer.h"

/**
 * Constructor (event listener set to NULL)
 */
omPlayer::omPlayer():ofxOMXPlayer()
{
	listener = NULL;
}


/**
 * Initialize the omPlayer
 *
 * \param textured set to true to enable textures
 * \param audioHDMI set to true to use HDMI for audio output
 * \see omPlayer()
 */
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

	//FRAMEBUFFERS
	if (settings.enableTexture)
	{
		//SHADERS
		blurV.load("VerticalBlur");
		blurH.load("HorizontalBlur");

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


/**
 * Set an event listener.
 *
 * \see omListener
 */
void omPlayer::setListener(omListener* myListener)
{
	this->listener = myListener;
}


/**
 * Apply volume settings
 *
 * \see params_t
 */
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

void omPlayer::iceBreak()
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
			if (freeze++ > 50)  
			{
				listener->onVideoFreeze();
				ofLog(OF_LOG_ERROR, "ICE BROKEN");
				freeze = 0;
				lastFrame = 0;
			}
		}
		else freeze = 0;
	}
	lastFrame = currentFrame;
}


/**
 * Create display rectangle.
 *
 * \see dims_t
 */
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


/**
 * Draw a frame(buffer) on screen.
 *
 * \see clearscreen()
 * \see blur()
 */
void omPlayer::show()
{
	//APPLY NEW VOLUME / MUTE
	this->applyVolume();

	//CHECK IF PLAYING
	if (!this->isPlaying()) return;

	//ANTI FREEZE
	this->iceBreak();

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


/**
 * Clear the current screen/framebuffer.
 *
 * \see show()
 */
void omPlayer::clearscreen()
{
	if (!this->isTextureEnabled()) return;
	framebuffer.begin();
		ofClear(0,0,0,0);
	framebuffer.end();
}


/**
 * Apply blur to frame.
 *
 * \see params_t
 * \see show()
 * \see clearscreen()
 */
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


/**
 * Start the movie player by loading the given file.
 *
 * \param file to load
 * \see stop()
 * \see resume()
 * \see seek(int timemilli)
 */
void omPlayer::play(string file){
		
	/*if (this->isPlaying()) this->stop(); 
	this->settings.videoPath = file;
	this->setup(this->settings);
	*/
	this->loadMovie(file);
	this->setPaused(false);	
}


/**
 * Seek to the given position.
 *
 * \todo implementation
 * \param timemilli time in milliseconds
 * \see play(string file)
 * \see stop()
 */
void omPlayer::seek(int timemilli){
		
	//if (this->isPlaying())
	//int frame = this->timeToFrameMs(timemilli);
	//TODO
}


/**
 * Stop current playback and clear screen.
 *
 * \see pause()
 * \see play(string file)
 * \see seek(int timemilli)
 */
void omPlayer::stop()
{	
	this->close();
	this->clearscreen();
}


/**
 * Pause current playback.
 *
 * \see stop()
 * \see play(string file)
 * \see seek(int timemilli) 
 */
void omPlayer::pause(){
	if (this->isPlaying()) this->setPaused(true);
}


/**
 * Resume current playback from being paused.
 *
 * \see pause()
 * \see play(string file)
 * \see stop()
 * \see seek(int timemilli)
 */
void omPlayer::resume(){
	if (this->isPlaying()) this->setPaused(false);
}


/**
 * Set volume for playback.
 *
 * \see setMuted(bool mute)
 * \see params_t
 */
void omPlayer::volume(int v){
	params.volume = v;
}


/**
 * Set mute status for playback.
 *
 * \param mute to set state (true = mute)
 * \see volume(int v)
 * \see params_t
 */
void omPlayer::setMuted(bool mute){
	params.mute = mute;
}

/**
 * Set amount of blurring.
 *
 * \param blur amount of blurring
 * \see setZoom(int zoom)
 * \see params_t
 */
void omPlayer::setBlur(int blur)
{
	this->params.blur = blur;
}


/**
 * Set amount of zooming.
 * 
 * \param zoom in percent
 * \see setBlur(int blur)
 * \see params_t
 */
void omPlayer::setZoom(int zoom)
{
	this->params.zoom = zoom;
}


/**
 * Retrieve current frame number
 *
 * \returns current frame number
 * \see getPositionMs()
 * \see timeToFrameMs(int timemilli)
 * \see getDurationMs()
 */
int omPlayer::getCurrentFrameNbr(){
	return (this->getCurrentFrame() % this->getTotalNumFrames());
}


/**
 * Retrieve current position in milliseconds.
 *
 * \returns current position in milliseconds
 * \see getCurrentFrameNbr()
 * \see getDurationMs()
 * \see timeToFrameMs(int timemilli)
 */
int omPlayer::getPositionMs(){
	return static_cast<int>( (double) this->getCurrentFrameNbr() * this->getDurationMs() / this->getTotalNumFrames());
}


/**
 * Get playback duration.
 *
 * \returns duration in milliseconds
 * \see getPositionMs()
 * \see timeToFrameMs(int timemilli)
 */
int omPlayer::getDurationMs(){
	return static_cast<int>(this->getDurationInSeconds()*1000);
}


/**
 * Calculate the number of frames shown in a given timespan.
 *
 * \param timemilli timespan in milliseconds
 * \returns number of frames per given timespan
 * \see getDurationMs()
 * \see getPositionMs()
 * \see getCurrentFrameNbr()
 * \todo reality check
 */
int omPlayer::timeToFrameMs(int timemilli){
    // number of frames in given timespan
	int frame = static_cast<int>( (double) timemilli * this->getTotalNumFrames() / this->getDurationMs());
    // ?
	return (frame % this->getTotalNumFrames());
}

