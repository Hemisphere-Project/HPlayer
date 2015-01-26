#pragma once
#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofFbo.h"

struct dimsIMG_t {
    int height;
    int width;
    int marginX;
    int marginY;
};

struct paramsIMG_t {
    int     blur;
    int     zoom;
    bool textured;
};

class imgPlayer 
{
    
    public:

        //RUN
        imgPlayer();
        void init(bool textured);
        void show();
        
        //CONTROL
        void play(string file);
        void stop();

        void setZoom(int zoom);
        void setBlur(int blur);
        
        //STATE
        bool    isPlaying();

        
    private:
                           
        void makeRect();
        void clearscreen();
        
        //FX
        void blur();
        
        //BUFFER & SHADER
        ofShader blurH, blurV;
        ofFbo framebuffer, frameblur;   

        ofImage* player;
        dimsIMG_t          dim;
        paramsIMG_t        params; 
};