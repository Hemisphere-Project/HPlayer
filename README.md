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

The player is build on top of OMXPlayer (using jvcleave's ofxOmxPlayer) and OpenFrameworks,
it's GPU accelerated and offers OpenGL Shaders and Textures support.

It is now capable to play Audio (wav/mp3/aif/ogg) / Video (mp4/mov/avi in h264) / Image (bmp/jpg/jpeg/gif/tiff/png) files.

HPlayer is developped under the terms of GNU GPL v2.
You can find a copy of this license in the LICENSE file.

HPlayer can be configured / commanded using:
- OSC messages
- command line arguments (on player start)
- settings.xml file (created at first start in ./data)

HPlayer has playlist capability: you can supply multiple file to play, or just a directory path, the player will handle the playlist and loop over if asked to.

Only one GL effect is available for now, the Blur effect.
It's a proof-of-concept: feel free to develop more and pull request it !
When the effect level is set to 0, the shader is simply bypassed so there is no overload if you don't want to use the effects.

The player covers the whole screen with a black background and keep video aspect ratio so you don't need to bother with clearing the background like with OMXPlayer. There is also a zoom out function that allow resizing down the video to fit a specific display configuration (textured mode must be enabled)


Installation from binary (RASPBIAN Jessie)
-------------

If you are running a recent Raspbian Jessie, you can simply download the content of the bin-raspbian-armv6 folder with the last build. OpenFrameworks 0.9 is set to compile with armv6 instructions set under Raspbian (for various reason). The good point is that the binary is compatible with Raspberries 1, 2 and 3.

It's better to start from a full Jessie Raspbian (not the Lite one). 
If you want start from a Jessie Lite please follow "Install from source" since more dependencies are required.

Don't forget the data subfolder which contains the shaders. It will not work without this folder. 
We will include those extra files into the binary in a futur release for more convinience.
You will also need to install some dependencies (mainly libfreeimage3), 
and set the GPU memory split with at least 128mb (you can do it from raspi-config) !

You can use those commands to install dependencies and download the binaries into a HPlayer directory in your HOME folder.
```bash
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install subversion libfreeimage3 -y

svn export https://github.com/Hemisphere-Project/HPlayer/trunk/bin-raspbian-armv6 ~/HPlayer

sudo raspi-config
	# Expand filesystem
	# Split GPU memory to 128 or 256
	# Bootup in Console
	# Overclock if you want to (recommanded on RPi 1)
	# Reboot !
```

To test it:

```bash
# Download a Test media
mkdir ~/media && cd ~/media
wget http://download.openbricks.org/sample/H264/big_buck_bunny_480p_H264_AAC_25fps_1800K.MP4 -O bbb-sample.mp4

# Start HPlayer in autoplay mode
cd ~/HPlayer
./HPlayer --start 1
```


Installation from binary (ARCH)
-------------

Arch Linux allows OF 0.9 to compile with armv7 instructions set. The system seems lighter but i did not notice a blasting improvment in HPlayer performances (.. but i should conduct proper testings). 
You can find a binary for Arch in the bin-arch-armv7. Beware that this binary will only work with RPi 2 & 3 (not the 1st).

I yet have to provide the dependencies to run the binary.. coming soon !
Don't forget the data subfolder which contains the shaders. It will not work without this folder.
You will also need to install some dependencies (mainly libfreeimage3), and don't forget to setup the GPU memory split with at least 128mb or ideally 256mb (by editing /boot/config) !


Installation from source (RASPBIAN)
-------------

HPlayer is build on top of:
```
	* OpenFrameworks 0.9.x ARMv6/7 (armv6 under Raspbian / armv7 under Arch)
	* ofxOMXPlayer from jvcleave
	* ofxCrypto from jkosoy 
	* ofxArgParser from satoruhiga
```

---

####0. Setup and Update your RaspberryPi 
```bash
sudo raspi-config
	# Expand filesystem
	# Split GPU memory to 128 or 256
	# Overclock if you want to (recommanded)
sudo reboot
```

####1. Run install script
You can find a complete installation script here:
https://gist.github.com/Maigre/1a7f159a81b9f1f96eb1e1d40194ad7f

Or you can directly run it on the RPi with this command:
```bash
bash <(curl -s https://gist.github.com/Maigre/1a7f159a81b9f1f96eb1e1d40194ad7f)
```

####2. Compile it 
```bash
cd ~/openFrameworks/apps/myApps/HPlayer
make
```

####2. Test it 
```bash
cd ~/openFrameworks/apps/myApps/HPlayer/bin
./HPlayer [--args]
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
