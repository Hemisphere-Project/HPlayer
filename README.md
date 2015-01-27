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

You can simply download the content of the bin folder with the last build.
Don't forget the data subfolder which contains the shaders. It will not work without this folder. We will include those extra files into the binary in a futur release for more convinience.

The binary might need some dependencies, we will populate a list very soon.


Installation from source
-------------

HPlayer is build on top of:
```
	* OpenFrameworks 0.8.x ARMv6
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


####1. Download / Install OpenFrameworks ARMv6
>Instructions: http://www.openframeworks.cc/setup/raspberrypi/

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
You are also welcome to ask for new features or submit ideas and fix ;)

HPlayer can be configured with the data/settings.xml file.
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

	
EXEMPLE (assuming /home/pi/media contains some videos):
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


Credits
-------------

HPlayer is developped by the Hemisphere-Project Team
```
	++ Thomas Bohl ++
	++ Alain Barthelemy ++
	++ Jeremie Forge ++
```
And we thank the hard work of 
```
	++ RaspberryPi Creators ++
	++ OpenFrameworks Developpers ++
	++ OMXPlayer Developpers ++
	++ jvcleave ++
	++ jkosoy ++
	++ satoruhiga ++
```




