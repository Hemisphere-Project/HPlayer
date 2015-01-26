#include "imgPlayer.h"

imgPlayer::imgPlayer() {
    player = NULL;
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void imgPlayer::play(string file)
{
    this->stop();
    player = new ofImage();
    player->loadImage( file );
}

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

void imgPlayer::clearscreen()
{
    framebuffer.begin();
        ofClear(0,0,0,0);
    framebuffer.end();
}

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

bool imgPlayer::isPlaying()
{
    return (player != NULL);
}

//--------------------------------------------------------------
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

//--------------------------------------------------------------
void imgPlayer::setBlur(int blur)
{
    this->params.blur = blur;
}

//--------------------------------------------------------------
void imgPlayer::setZoom(int zoom)
{
    this->params.zoom = zoom;
}