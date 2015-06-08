#include "xmlSettings.h"

/**
 * Read setting from XML file.
 *
 * \param file file(path) to read settings from
 */
xmlSettings::xmlSettings(string file)
{
    confFile = file;
    loadFile(confFile);
}


/**
 * Set an integer value based on the default configuration in the XML file.
 *
 * \param node path to node
 * \param leaf node to configure
 * \param def default (type) value
 */
int xmlSettings::conf_int(string node,string leaf,int def)
{
    int val = getValue(node+":"+leaf,def);
    if (val == def) setValue(node+":"+leaf,val);
    saveFile(confFile);
    return val;
}


/**
 * Set a string value based on the default configuration in the XML file.
 *
 * \param node path to node
 * \param leaf node to configure
 * \param def default (type) value
 */
string xmlSettings::conf_str(string node,string leaf,const string &def)
{
    string val = getValue(node+":"+leaf,def);
    if (val == def) setValue(node+":"+leaf,val);
    saveFile(confFile);
    return val;
}


/**
 * Set a boolean value based on the default configuration in the XML file.
 *
 * \param node path to node
 * \param leaf node to configure
 * \param def default (type) value
 */
bool xmlSettings::conf_bool(string node,string leaf,bool def)
{
    bool val = (getValue(node+":"+leaf, (def)?1:0 ) == 1);
    if (val == def) setValue(node+":"+leaf, (val)?1:0 );
    saveFile(confFile);
    return val;
}
