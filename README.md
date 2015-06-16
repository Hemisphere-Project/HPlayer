```
 __  __  ____    ___                                     
/\ \/\ \/\  _`\ /\_ \                                    
\ \ \_\ \ \ \L\ \//\ \      __     __  __     __   _ __  
 \ \  _  \ \ ,__/ \ \ \   /'__`\  /\ \/\ \  /'__`\/\`'__\
  \ \ \ \ \ \ \/   \_\ \_/\ \L\.\_\ \ \_\ \/\  __/\ \ \/ 
   \ \_\ \_\ \_\   /\____\ \__/.\_\\/`____ \ \____\\ \_\ 
    \/_/\/_/\/_/   \/____/\/__/\/_/ `/___/> \/____/ \/_/ 
                                       /\___/            
                                       \/__/             
```

HPlayer OSC MediaPlayer for Raspberry Pi
-------------


What is it
-------------

HPlayer is a media player for RaspberryPi controllable with OSC commands.

The player is build on top of OMXPlayer and OpenFrameworks,
it's GPU accelerated and offers OpenGL Shaders and Textures support.

It is now capable to play Audio (wav/mp3/aif/ogg) / Video (mp4/mov/avi in h264) / Image (bmp/jpg/jpeg/gif/tiff/png) files.

HPlayer is developped under the terms of GNU GPL v2.
You can find a copy of this license in the LICENSE file.

HPlayer supports command line arguments to start the player with a given setup, so the use of OSC is not compulsory. You can still use it like the original OMXPlayer, but with more flexibility.
A settings file (in ./data) is also available to feed custom behavior without using the command line.

HPlayer has playlist capability: you can supply multiple file to play, or just a directory path, the player will handle the playlist and loop over if asked to.

Only one GL effect is available for now, the Blur effect.
It's a proof-of-concept and it will be extended to custom shaders later on.
When the effect level is set to 0, the shader is simply bypassed so there is no overload if you don't want to use the effects.

The player covers the whole screen with a black background and keep video aspect ratio so you don't need to bother with clearing the background like with OMXPlayer. There is also a zoom out function that allow resizing down the video to fit a specific display configuration (textured mode must be enabled)


Installation from binary
-------------

If you are running a recent Raspbian on your Pi 1 (armv6), you can simply download the content of the bin folder with the last build. Don't forget the data subfolder which contains the shaders. It will not work without this folder. We will include those extra files into the binary in a futur release for more convinience.
You will also need to install some dependencies (mainly libfreeimage3), and don't forget to setup the GPU memory split with at least 128mb (you can do it from raspi-config) !

You can use those commands to install dependencies and download the binaries into a HPlayer directory in your HOME folder.
```bash
sudo apt-get install subversion libfreeimage3
svn export https://github.com/Hemisphere-Project/HPlayer/trunk/bin ~/HPlayer
```


Installation from source
-------------

HPlayer is build on top of:
```
	* OpenFrameworks 0.9.x ARMv6/7
	* ofxOMXPlayer from jvcleave
	* ofxCrypto from jkosoy 
	* ofxArgParser from satoruhiga
```

---

####0. Setup and Update your RaspberryPi 
```bash
sudo apt-get update && sudo apt-get upgrade
sudo apt-get install git 
sudo raspi-config
	# Expand filesystem
	# Split GPU memory to 128 or 256
	# Overclock if you want to (recommanded)
sudo rpi-update
sudo reboot
```


####1. Download OpenFrameworks, Install dependencies and patch ofxOSC

OF 0.8.4 is the current stable release, but doesn't feature a key change that appears in 0F 0.9 future release: the possibility to disable Bundles while forging OSC messages via the now embbeded ofxOsc addon.
You could download the last OF from github but for simplicity and stability we will use here a patched version of the current 0.8.4 stable OF version. The patch will become obsolete as soon as OF 0.9 is released.

**Download and install OF**
on RPI1 (armv6): [http://openframeworks.cc/setup/raspberrypi/Raspberry-Pi-Getting-Started.html] 
```bash
cd
curl -O http://www.openframeworks.cc/versions/v0.8.4/of_v0.8.4_linuxarmv6l_release.tar.gz
mkdir openFrameworks
tar vxfz of_v0.8.4_linuxarmv6l_release.tar.gz -C openFrameworks --strip-components 1
cd ~/openFrameworks/scripts/linux/debian_armv6l
sudo ./install_dependencies.sh
make Release -C ~/openFrameworks/libs/openFrameworksCompiled/project
```

on RPi2 (armv7): [http://forum.openframeworks.cc/t/raspberry-pi-2-setup-guide/18690] 
```bash
cd
curl -O http://www.openframeworks.cc/versions/v0.8.4/of_v0.8.4_linuxarmv7l_release.tar.gz
mkdir openFrameworks
tar vxfz of_v0.8.4_linuxarmv7l_release.tar.gz -C openFrameworks --strip-components 1
curl https://raw.githubusercontent.com/openframeworks/openFrameworks/master/libs/openFrameworksCompiled/project/linuxarmv7l/config.linuxarmv7l.rpi2.mk -o ~/openFrameworks/libs/openFrameworksCompiled/project/linuxarmv7l/config.linuxarmv7l.rpi2.mk
cd ~/openFrameworks/scripts/linux/debian
sudo ./install_dependencies.sh
export MAKEFLAGS=-j4 PLATFORM_VARIANT=rpi2
cd ~/openFrameworks/examples/3d/3DPrimitivesExample/
make
```

**Patch ofxOsc** (if you are using an OF version lower than 0.9):
```bash
curl https://gist.githubusercontent.com/Maigre/a87adb190943903bbf66/raw/611a038216508e42b012da85705de150667d392b/gistfile1.cpp | patch -N
```

####2. Clone Addons ofxOMXPlayer / ofxArgParser / ofxCrypto
```bash
cd ~/openFrameworks/addons/ 
git clone https://github.com/jvcleave/ofxOMXPlayer.git
git clone https://github.com/satoruhiga/ofxArgParser.git
git clone https://github.com/jkosoy/ofxCrypto.git
```

####3. Clone project HPlayer
```bash
cd ~/openFrameworks/apps/myApps/
git clone https://github.com/Hemisphere-Project/HPlayer.git
```

####4. You are now ready to compile the player.
```bash
cd ~/openFrameworks/apps/myApps/HPlayer
make
```

####5. Test it 
```bash
./bin/Hplayer [--args]
```
You can copy the content of the ./bin directory to your usual binary path (don't forget the data subdirectory with the shaders !).
We will provide a "make install" routine as soon as the player become more stable.


Use it!
-------------

Be Aware that HPlayer is still a work in progress, and some features could 
have major or minor bugs. 
Please use the Issue report on GitHub if you find some ! 
You are also welcome to ask for new features or submit ideas and fixes ;)

HPlayer can be configured with the **data/settings.xml** file.
If this file is missing, it will be created on first start by the player with default values.

HPlayer also supports various optional command line arguments 
(which will overwrite the values in settings.xml):

	//PLAYER
	--name <nick>		: Name your player instance (default=HPlayer)		
	--info <0:1>		: Display info window	(default=0)	

	//SOUND
	--volume <0:100>	: Volume (default=50)
	--ahdmi <0:1>		: Use HDMI audio output	(default=0)

	//OPENGL TEXTURED MODE
	--gl <0:1>			: Enable GL mode 			(default=1)
	--zoom <0:100>		: Zoom out (GL enabled)		(default=100)
	--blur <0:100>		: Blur level (GL enabled)	(default=0)
	
	//PLAYBACK
	--path <path>		: Basepath for media with relative path (default=/home/pi/media)
	--start <0:1> 		: Start playback of the basepath directory (default=0)
	--media <path>		: Play this specific File or Directory now (default="")
	--loop <0:1>		: Loop upon playlist (default=1)

	//OSC
	--in <port> 		: Input OSC Port (default=9000)
	--out <port> 		: Output OSC Port (default=5000)
	--host <ip>			: Send message to host (default=localhost)
	--base64 <0:1>		: Base64 encoding for filepath (default=0)
	--cmdmap <name>		: Use the OSC command map in settings.xml (default=default)
	--prefix <prefix>	: Default prefix before OSC message (default="")

	
EXAMPLE (assuming /home/pi/media contains some medias):
```bash
./HPlayer --path /home/pi/media --start 1 --loop 1 --zoom 50 --info 1
```

Once started, HPlayer can be controlled with OSC commands over the network.

The available OSC commands are:

	/play [<path1>] [<path2>] ...		: Play the file (or dir) list in order
	/playloop [<path1>] [<path2>] ...	: Same as play with playlist loop forced
	/load [<path1>] [<path2>] ...		: Pre-Load a playlist
	/volume <0:100>		: Set volume from 0 to 100
	/blur <0:100>		: Set blur level from 0 to 100%
	/zoom <0:100>		: Set zoom from 0 to 100%
	/stop			: Stop and rewind the current playlist
	/pause			: Pause the current file
	/resume			: Resume the paused file
	/next			: Play the next file in the list
	/prev			: Play the previous file in the list
	/mute			: Mute the sound
	/unmute			: Unmute the sound
	/loop			: Enable looping for the current playlist
	/unloop			: Disable looping for the current playlist
	/info			: Toggle media info window
	/host <ip>		: Change IP for OSC out
	/getStatus		: Ask for status info
	/quit			: Exit the player

References
----------
* [omxplayer github repository](https://github.com/popcornmix/omxplayer)
* [elinux: omxplayer](http://elinux.org/Omxplayer)
* [doxygen](http://www.stack.nl/~dimitri/doxygen/) (documentation framework)
* [wikipedia: OpenMAX](https://en.wikipedia.org/wiki/OpenMAX)


Credits
-------------

HPlayer is developped by the Hemisphere-Project Team
```
	++ Thomas Bohl ++
	++ Alain Barthelemy ++
	++ Jeremie Forge ++
```
We thank the hard work of 
```
	++ RaspberryPi Creators ++
	++ OpenFrameworks Developpers ++
	++ OMXPlayer Developpers ++
	++ jvcleave ++
	++ jkosoy ++
	++ satoruhiga ++
```
We also thank the contributors to this project
```
	++ tpltnt ++
```
