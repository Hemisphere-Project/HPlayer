#pragma once
#include "ofImage.h"
#include "ofAppRunner.h"
#include "ofGraphics.h"
#include "ofShader.h"
#include "ofFbo.h"

struct dimsIMG_t {
    int height; /*!< height of the display (screen) */
    int width; /*!< width of the display (screen) */
    int marginX; /*!< horizontal display margin */
    int marginY; /*!< vertical display margin */
};

struct paramsIMG_t {
    int     blur; /*!< indicate amount of blur */
    int     zoom; /*!< level of zoom (in percent) */
    bool textured; /*!< indicate use of effects */
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
        ofShader blurH; /*!< shader for horizontal blurring */
        ofShader  blurV; /*!< shader for vertical blurring */
        ofFbo framebuffer; /*!< basic framebuffer */
        ofFbo  frameblur; /*!< framebuffer for blurring */

        ofImage* player; /*!< container class for image (present during playback) */
        dimsIMG_t          dim; /*!< screen/display dimensions */
        paramsIMG_t        params; /*!< parameters for displaying images */
};
