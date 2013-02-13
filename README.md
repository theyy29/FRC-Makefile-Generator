FRC-Makefile-Generator
======================
A custom tool to create the makefile instead of WindRiver. Also comes with a compile script. And a script to ftp the file to the robot.

Usage:
Windows:
    Prerequisites:
        WindRiver
        -- or --
        A compiled set of the gcc toolset (and binutils, both configured to cross-compile to vx-works) AND the vx-works includes and libraries.
    Copy the executable, and the nessasary library files (unless they are added you your path)
        along with the resource files to your project compilation folder (ie. C:/WindRiver/workspace/2013-FRC-Robot/PPC603gnu/)
    run compile.bat
Linux:
    Prerequisites:
        Using WindRiver under wine, using the windows versions of these tools (messy, but do-able)
        -- or --
        A compiled set of the gcc toolset (and binutils, both configured to cross-compile to vx-works) AND the vx-works includes and libraries.
    Copy the executable, and the nessasary library files (unless they are added you your path)
        along with the resource files to your project compilation folder (ie. /home/username/Development/2013-FRC-Robot/PPC603gnu)
    run compile.sh
    
    
Files it depends on:
mkp1 : first part of the makefile, before any generated content.
mkp2 : middle section of the makefile, in between generated content.
mkp3 : last part of the makefile, after all generated content.
Found in the Resources directory


Nessasary library files:
Windows:
  mingwm10.dll
  QtCored4.dll
  libgcc_s_dw2-1.dll
Linux:

Goals:
Make it more customizable,
and less dependant on users changing content in the file.

support for linux.
support for osx?

Other:
Developed in Qt-Creator, compiled with qmake/make/mingw
Developed by Ethan S