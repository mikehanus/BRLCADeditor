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
    rcc("outer.s", 0, 0, 0, 0, 0, 10, 5);
    rcc("inner.s", 0, 0, 0, 0, 0, 10, 4);
    tor("bottomTor.s", 0, 0, 0, 0, 0, 1, 4, 1);
    rcc("bottom.s", 0, 0, -1, 0, 0, 1, 4);
    tor("topTor.s", 0, 0, 10, 0, 0, 1, 4.5, 0.5);
    tor("ear.s", 0, 5, 5, 1, 0, 0, 3.5, 0.5);
    combinationsManual("cup.r", true, "{- {u {u {l outer.s} {l bottom.s}} {u {u {l bottomTor.s} {l topTor.s}} {l ear.s}}} {l inner.s}}");

    color("cup.r", 255, 255, 0);

    combinationsManual("caffee.r", true, "{l " + rcc("caffee.s", 0, 0, 0, 0, 0, 8, 4) + "}");
	
    attrManual("caffee.r", " {color} {200/100/0} {shader} {glass {tr 0.2} ");

    combinationsManual("all.g", false, "{u {l cup.r} {l caffee.r}}");    

    saveASC("cup.asc");
}
