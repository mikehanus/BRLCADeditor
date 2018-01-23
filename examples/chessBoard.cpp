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

int main()
{
    title("chess board using c++ script by Michal Hanus");
    units("mm");
    int size = 57, height = 10, border = 10;
    string whiteCells, blackCells, tmpName;

    for(int x = 0; x < 8; x += 1)
    {
        for(int y = 0; y < 8; y += 1)
        {
            tmpName = "cube" + to_string(x) + to_string(y) + ".s";
            rpp(tmpName, x*size,(x+1)*size, y*size,(y+1)*size, 0,height);

            if(x == 0 && y == 0) blackCells = "{l " + tmpName  + "}";
            else if(x == 0 && y == 1) whiteCells = "{l " + tmpName  + "}";

            else if((x + y) % 2) whiteCells = "{u " + whiteCells + " {l " + tmpName + "}}";
            else                 blackCells = "{u " + blackCells + " {l " + tmpName + "}}";
        }
    }


    rpp("borderInner.s", 0,8*size, 0,8*size, 0,10);
    rpp("borderOuter.s", -border,(8*size)+border, -border,(8*size) + border, 0,10);
    combinationsManual("border.r", true, "{- {l borderOuter.s} {l borderInner.s}}");
    color("border.r", 175, 120, 23);


    combinationsManual("whiteCells.r", true, whiteCells);
    color("whiteCells.r", 255, 255, 255);
    combinationsManual("blackCells.r", true, blackCells);
    color("blackCells.r", 0, 0, 0);


    combinationsManual("all.g", false, "{u {u {l whiteCells.r} {l blackCells.r}} {l border.r}}");

    saveASC("chessBoard.asc");
}
