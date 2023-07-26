Port for PSVita of Zik-Zak.
Thanks to CatoTheYounger for testing the port.
vitaGL needs HAVE_DISPLAY_LISTS=1.

Original readme:

 Zik Zak v0.1 - 2006-06-17
--------------------------------------------------------

 Program by Andreas Textor
 Music by Martin Textor
--------------------------------------------------------

 1. How to play
--------------------------------------------------------

The goal of each level is to reach the next level.

Use the mouse to adjust the view (look around).
- Left Mouse Button / Space Bar:  Shoot
- WASD / Arrow Keys:              Shift view
- R:                              Reset view
- ESC:                            Quit

Whenever you reach a new level, the game is saved, so
you don't have to play all the levels before again when
you run the game next time.


 2. How to compile (if you use the Windows binary
    package you can skip this section)
--------------------------------------------------------

Linux:
 You need to have SDL and SDL_mixer (and the corresponding
 -dev packages) installed.
 Type 'make' to compile the game, then ./zikzak to run it.

Windows:
 Things are a bit more complicated. I assume you have
 compiled a SDL program before using mingw:
 Set up msys and mingw, install SDL and SDL_mixer libs
 and OpenGL headers in mingw, then in the msys console
 cd into the source directory and type
 'make -f Makefile.mingw'. If these instructions are
 not enough, email me, I'm just too lazy now to go into
 details.

 3. Configuration file
--------------------------------------------------------

You can set the resolution and fullscreen 0 (for off)
or 1 (for on) in the config file, and if you wish to
mute the music during the game (also 0 or 1).

Linux:
The configuration file is ~/.zikzakrc.

Windows:
The configuration file (zikzak.conf) can be found in
the game directory.

 4. Troubleshooting
--------------------------------------------------------

Q: I'm using the windows version and the window pops
   up and closes immediately
A: See the stderr.txt file in the game directory.
   Maybe a file was not found (in that case, run the
   .exe from the right directory) or your graphics
   drivers are not up to date.

Q: The same thing happens, but I'm using Linux, and
   there's no stderr.txt file.
A: Start the game from a text console and see the output.

 5. License
--------------------------------------------------------

I release the program under the terms of the GNU General
Public License Version 2 or later.
Go to http://www.gnu.org/copyleft/gpl.html to read
the license.

The music is published under the terms of the Creative
Commons Attribution-NonCommercial-ShareAlike 2.5 License,
(http://creativecommons.org/licenses/by-nc-sa/2.5/)
which basically means you can share it as you like but
not use it commercially.

The sound effects were taken from
http://www.grsites.com/sounds/ and are free to use
for personal use.

 6. Contact
--------------------------------------------------------

See http://kantico.de/ for updates and the source code
(if you don't have it right here).
Mail me at a.textor@kantico.de for comments.

