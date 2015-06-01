#include "imgPlayer.h"


/**
 * Plan constructor.
 */
imgPlayer::imgPlayer() {
    player = NULL;
}

/**
 * Initialize a image player with default values.
 *
 * \param textured to indicate application
 */
void imgPlayer::init(bool textured)
{   
    //params DEFAULTS
    params.blur = 0;
    params.zoom = 100;
    params.textured = textured;
    
    //FRAMEBUFFERS
    if (params.textured)
    {
        //SHADERS
        blurV.load("VerticalBlur");
        blurH.load("HorizontalBlur");

        framebuffer.allocate(ofGetWidth(), ofGetHeight());
        frameblur.allocate(ofGetWidth(), ofGetHeight());
        this->clearscreen();
    }
}


/**
 * Play/load a given file.
 *
 * \param file to load
 * \see show()
 */
void imgPlayer::play(string file)
{
    this->stop();
    player = new ofImage();
    player->loadImage( file );
}


/**
 * Display the content of the current frame(buffer).
 *
 * \see clearscreen()
 * \see blur()
 * \see makeRect()
 * \see play(string file)
 */
void imgPlayer::show()
{
    if (!this->isPlaying()) return;

    //CALCULATE DIMS
    this->makeRect();

    if (params.textured)
    {
        //CLEAR BUFFER
        this->clearscreen();

        //FILL BUFFER
        framebuffer.begin();
            player->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
        framebuffer.end();

        //BLUR
        if (params.blur > 0) this->blur();  

        //DRAW TO SCREEN
        framebuffer.draw(0, 0);
    }
    else player->draw(this->dim.marginX, this->dim.marginY, this->dim.width, this->dim.height);
}

/**
 * Create display rectangle.
 *
 * \see show()
 */
void imgPlayer::makeRect()
{
    //WIDTH
    dim.width = floor( ofGetHeight() * player->getWidth() / player->getHeight() ); //WIDTH RATIO FROM MAXIMIZED HEIGHT
    if (dim.width > ofGetWidth()) dim.width = ofGetWidth(); //SHRINK IF WIDTH IS TO BIG
    
    //ZOOM
    if (params.zoom != 100)
    {
        dim.width = floor( dim.width * params.zoom / 100. ); //APPLY ZOOM
        if (dim.width > ofGetWidth()) dim.width = ofGetWidth(); //SHRINK IF WIDTH IS TO BIG
    }
    if (!(dim.width > 0)) return;
    
    //HEIGHT 
    dim.height = floor( dim.width * player->getHeight() / player->getWidth() ); //KEEP ASPECT RATIO
    if (!(dim.height > 0)) return;
    
    //MARGINS
    dim.marginX = floor((ofGetWidth()-dim.width)/2);
    dim.marginY = floor((ofGetHeight()-dim.height)/2);
}


/**
 * Clear the current screen/framebuffer.
 *
 * \see show()
 * \see blur()
 */
void imgPlayer::clearscreen()
{
    framebuffer.begin();
        ofClear(0,0,0,0);
    framebuffer.end();
}


/**
 * Apply blur to framebuffer.
 *
 * \see show()
 * \see clearscreen()
 */
void imgPlayer::blur()
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
 * Check if there is currently a playback going on.
 *
 * \returns indication of playback
 * \see stop()
 */
bool imgPlayer::isPlaying()
{
    return (player != NULL);
}


/**
 * Stop current playback.
 *
 * \see isPlaying()
 */
void imgPlayer::stop()
{   
    if (player != NULL)
    {
        player->clear();
        delete player;
        player = NULL;

        if (params.textured) this->clearscreen();
    }
}


/**
 * Set amount of blur.
 *
 * \param blur amount
 * \see blur()
 * \see clearscreen()
 */
void imgPlayer::setBlur(int blur)
{
    this->params.blur = blur;
}


/**
 * Set the level of zoom.
 *
 * \param zoom level in percent
 * \see clearscreen()
 */
void imgPlayer::setZoom(int zoom)
{
    this->params.zoom = zoom;
}
