# BRL-CAD editor

This program was made for BRL-CAD community (www.brlcad.org) as a part of Google Code-In 2017 contest. 

The idea behind it was to enhance BRL-CAD program interface with simple script-editing features in C++ and ASCII formats, similarly to OpenSCAD (http://www.openscad.org/) style of editing. 
DXF and STL import/export functions were included to this editor as well for convenience.

## Video summary of the program features  
See (https://youtube.com/)

## Warning
This project is still in its early development phase and probably pretty buggy, so if you are looking for something stable and working, this would not be a good choise for you :-)
But if you are brave enough and advanced in BRL-CAD use, please make a try.
Any inputs for corrections and inovations are welcome!
 
## License
This program is licensed under BSD-3-Clause license. 

## Installation:
1. Install BRL-CAD: [download install file](https://sourceforge.net/projects/brlcad/files/) or [compile from source](http://brlcad.org/wiki/SVN)
2. Install Qt5 (https://www.qt.io/download) or use `apt` in debian-based OS.
3. `git clone https://github.com/mikehanus/BRLCADeditor.git` or download it as a zip and unpack it
4. `cd BRLCADeditor`
5. Open `main.cpp`, find line 10 and replace "/usr/brlcad/bin/" with your BRL-CAD bin path (there are programs like g-step, rt, dxf-g, etc... in it)
6. `qmake`
7. `make`
8. `./BRLCADeditor` or `BRLCADeditor.exe` (If you would make it successfully on Windows please let me know, I used in only on Debian 9)

## Using C++ script
Find `primitives.h` in `lib` subfolder. 
Link this header file to your code using #include path (or copy it to the same folder as your code) 
For the code style, see `examples` subfolder and `primitives.h`
Please note that only tgc, rpp, tor and arb8 primitives are included so far, adding the others is up to you :)

## Features
1. Open g-database and automatically convert it to ASCII
2. Edit ASCII or OpenSCAD-like Cpp scripts and use raytrace and view rotations using sliders
3. Import DXF or STL file (Ctrl+I) and insert it to an existing file 
4. Export DXF and STL files (Ctrl+E) 

## For questions and comments please contact
Michal Hanus <mikehanus(at)protonmail.com>

