#include "xmlSettings.h"

xmlSettings::xmlSettings(string file)
{
    confFile = file;
    loadFile(confFile);
}

int xmlSettings::conf_int(string node,string leaf,int def)
{
    int val = getValue(node+":"+leaf,def);
    if (val == def) setValue(node+":"+leaf,val);
    saveFile(confFile);  
    return val;
}

string xmlSettings::conf_str(string node,string leaf,const string &def)
{
    string val = "";
    val = getValue(node+":"+leaf,def);
    if (val == def) setValue(node+":"+leaf,val);
    saveFile(confFile);
    return val;
}

bool xmlSettings::conf_bool(string node,string leaf,bool def)
{
    bool val = (getValue(node+":"+leaf, (def)?1:0 ) == 1);
    if (val == def) setValue(node+":"+leaf, (val)?1:0 );
    saveFile(confFile);
    return val;
}
