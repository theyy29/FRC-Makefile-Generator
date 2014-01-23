FRC-Makefile-Generator
======================
A custom tool to create the makefile instead of WindRiver. Also comes with a compile script. And a script to ftp the file to the robot.

Usage:
Windows:
    Prerequisites:
        WindRiver
        -- or --
        A compiled set of the gcc toolset (and binutils, both configured to cross-compile to vx-works) AND the vx-works includes and libraries.
    Copy the executable (MakeGen), and the nessasary library files (unless they are added you your path)
        along with the resource files to your project compilation folder (ie. C:/WindRiver/workspace/2013-FRC-Robot/PPC603gnu/)
    run compile.bat
Linux:
    Prerequisites:
        Using WindRiver under wine, using the windows versions of these tools (messy, but do-able)
        -- or --
        A compiled set of the gcc toolset (and binutils, both configured to cross-compile to vx-works) AND the vx-works includes and libraries.
    Copy the executable (MakeGen), and the nessasary library files (unless they are added you your path)
        along with the resource files to your project compilation folder (ie. /home/username/Development/2013-FRC-Robot/PPC603gnu)
    run compile.sh
    

Nessasary library files (unneeded if they are in your path):
Windows:
  mingwm10.dll
  QtCored4.dll
  libgcc_s_dw2-1.dll
Linux:
  All libraries should be in the path, unless you are using a non-graphical linux distro.
  Then it would probably be the .so equivilents.
OSX:
  Not supported yet. Do not know.

Goals:
Make it more customizable (even more),

debug/release (ftp commands, build)

add generation of the ftp_commands to the program as an option.

support for linux.
support for osx?

Other:
Developed in Qt-Creator, compiled with qmake/make/mingw
Developed by Ethan S
