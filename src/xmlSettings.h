#pragma once
#include "ofxXmlSettings.h"

/**
 * A class to handle (default) XML settings. It is based on
 * [ofxXmlSettings](http://www.openframeworks.cc/documentation/ofxXmlSettings/ofxXmlSettings.html).
 */
class xmlSettings : public ofxXmlSettings
{
    
    public:
        xmlSettings(string file);
        int     conf_int(string node,string leaf,int def);
        string  conf_str(string node,string leaf,const string &def);
        bool    conf_bool(string node,string leaf,bool def);

    private:
        string confFile;
};
