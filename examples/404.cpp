/*
 * Copyright (C) 2018 Michal Hanus
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "primitives.h"
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

string four(double startx, double sizex, double baseStartx, double baseSizex, double starty, double sizey, double centerStarty, double centerSizey, double connectSize, double sizez,double startz,int number);

int main()
{
    title("404 using c++ script by Michal Hanus");
    units("mm");

    four(0, 70, 50, 10, 0, 100, 30, 10, 14.14, 0, 10, 1);
    four(200, 70, 50, 10, 0, 100, 30, 10, 14.14, 0, 10, 2);

    tgc("outer.s", 140, 50, 0,  0, 0, 10, 
                   0, -50, 0,   35, 0, 0,
                   0, -50, 0,   35, 0, 0);

    tgc("inner.s", 140, 50, 0,  0, 0, 10, 
                   0, -40, 0,   25, 0, 0,
                   0, -40, 0,   25, 0, 0);

    combinationsManual("zero.r", true, "{- {l outer.s} {l inner.s}}");

    color("four1.r", 255, 0, 0);
    color("zero.r" , 0, 255, 0);
    color("four2.r", 0, 0, 255);

    combinationsManual("all.g", false, "{ u {u {l four1.r} {l four2.r}} {l zero.r}}");
    
    saveASC("404.asc");
}

string four(double startx, double sizex, double baseStartx, double baseSizex, double starty, double sizey, double centerStarty, double centerSizey, double connectSize, double startz, double sizez,int number)
{
    string n = to_string(number);
    rpp("fourBase" + n + ".s", startx + baseStartx, startx + baseStartx + baseSizex,   starty, starty + sizey,  startz, startz + sizez);
    rpp("fourCenter" + n + ".s", startx, startx + sizex, centerStarty, centerStarty + centerSizey, startz, startz + sizez);
    
    arb8("fourConnect" + n + ".s",  startx,                     centerStarty + centerSizey,   startz,          
                                    startx + connectSize,       centerStarty + centerSizey,   startz,
                                    startx + connectSize,       centerStarty + centerSizey,   startz + sizez,  
                                    startx,                     centerStarty + centerSizey,   startz + sizez,
                                    startx + baseStartx,        starty + sizey,               startz,                   
                                    startx + baseStartx,        starty + sizey - connectSize, startz, 
                                    startx + baseStartx,        starty + sizey - connectSize, startz + sizez,           
                                    startx + baseStartx,        starty + sizey,               startz + sizez);

    return combinationsManual("four" + n + ".r", true, "{u {u {l fourBase" + n + ".s} {l fourCenter" + n + ".s}} {l fourConnect" + n + ".s}}");
}
