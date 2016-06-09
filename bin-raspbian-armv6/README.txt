 __  __  ____    ___                                     
/\ \/\ \/\  _`\ /\_ \                                    
\ \ \_\ \ \ \L\ \//\ \      __     __  __     __   _ __  
 \ \  _  \ \ ,__/ \ \ \   /'__`\  /\ \/\ \  /'__`\/\`'__\
  \ \ \ \ \ \ \/   \_\ \_/\ \L\.\_\ \ \_\ \/\  __/\ \ \/ 
   \ \_\ \_\ \_\   /\____\ \__/.\_\\/`____ \ \____\\ \_\ 
    \/_/\/_/\/_/   \/____/\/__/\/_/ `/___/> \/____/ \/_/ 
                                       /\___/            
                                       \/__/             

HPlayer OSC MediaPlayer for Raspberry Pi
----------------------------------------

What is it
----------

HPlayer is a media player for RaspberryPi controllable with OSC commands.

The player is build on top of OMXPlayer and OpenFrameworks, it's GPU accelerated and offers OpenGL Shaders and Textures support.

It is now capable to play Audio (wav/mp3/aif/ogg) / Video (mp4/mov/avi in h264) / Image (bmp/jpg/jpeg/gif/tiff/png) files.

HPlayer is developped under the terms of GNU GPL v2. You can find a copy of this license in the LICENSE file.

HPlayer supports command line arguments to start the player with a given setup, so the use of OSC is not compulsory. You can still use it like the original OMXPlayer, but with more flexibility. A settings file (in ./data) is also available to feed custom behavior without using the command line.

HPlayer has playlist capability: you can supply multiple file to play, or just a directory path, the player will handle the playlist and loop over if asked to.

Only one GL effect is available for now, the Blur effect. It's a proof-of-concept and it will be extended to custom shaders later on. When the effect level is set to 0, the shader is simply bypassed so there is no overload if you don't want to use the effects.

The player covers the whole screen with a black background and keep video aspect ratio so you don't need to bother with clearing the background like with OMXPlayer. There is also a zoom out function that allow resizing down the video to fit a specific display configuration (textured mode must be enabled)


How to
------

Please read the updated documentation at https://github.com/Hemisphere-Project/HPlayer



