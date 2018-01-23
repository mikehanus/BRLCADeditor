/* Copyright 2018 Michal Hanus
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef BRLCAD_SCRIPT_PRIMITIVES
#define BRLCAD_SCRIPT_PRIMITIVES
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#define rpp(name, Xmin, Xmax, Ymin, Ymax, Zmin, Zmax) arb8(name, Xmin,Ymin,Zmin,  Xmax,Ymin,Zmin, Xmax,Ymax,Zmin, Xmin,Ymax,Zmin, Xmin,Ymin,Zmax, Xmax,Ymin,Zmax, Xmax,Ymax,Zmax, Xmin,Ymax,Zmax)
#define rcc(name, x, y, z, hx, hy, hz, r) tgc(name, x, y, z, hx, hy, hz, 0, -r, 0, r, 0, 0, 0, -r, 0, r, 0, 0)

string generateSomeRandomName();
string title(string value);
string units(string value);
string arb8(string name, double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3, double z3, double x4, double y4, double z4, double x5, double y5, double z5, double x6, double y6, double z6, double x7, double y7, double z7, double x8, double y8, double z8);
string tgc ( string name, double vertexx, double vertexy, double vertexz, double heightx, double heighty, double heightz, double ax, double ay, double az, double bx, double by, double bz, double cx, double cy, double cz, double dx, double dy, double dz);

string color(string name, int R, int G, int B);
string attrManual(string name, string attrValue);
void Manual(string line);
string combinationsManual(string name, bool region, string tree);

void saveASC(string path);

//source

string PrmContent;
int gsrnCount = 0;

string generateSomeRandomName()
{
    return string("gsrn") + to_string(gsrnCount++);
}

string title(string value)
{
    PrmContent += "title {" + value + "}\n";
    return "";
}

string units(string value)
{
    PrmContent += "units " + value + "\n";
    return "";
}

string arb8
(
 string name,
 double x1, double y1, double z1,
 double x2, double y2, double z2,
 double x3, double y3, double z3,
 double x4, double y4, double z4,
 double x5, double y5, double z5,
 double x6, double y6, double z6,
 double x7, double y7, double z7,
 double x8, double y8, double z8
)
{
    stringstream ss;
    ss << "put {" << name << "} arb8 V1 {" << x1 << " " << y1 << " " << z1
                              << " } V2 {" << x2 << " " << y2 << " " << z2
                              << " } V3 {" << x3 << " " << y3 << " " << z3
                              << " } V4 {" << x4 << " " << y4 << " " << z4
                              << " } V5 {" << x5 << " " << y5 << " " << z5
                              << " } V6 {" << x6 << " " << y6 << " " << z6
                              << " } V7 {" << x7 << " " << y7 << " " << z7
                              << " } V8 {" << x8 << " " << y8 << " " << z8 << "}\n";
    PrmContent += ss.str();
    return name;
}

string tgc
(
 string name,
 double vertexx, double vertexy, double vertexz,
 double heightx, double heighty, double heightz,
 double ax, double ay, double az,
 double bx, double by, double bz,
 double cx, double cy, double cz,
 double dx, double dy, double dz
)
{
    stringstream ss;
    ss << "put {" << name << "} tgc V { " << vertexx << " " << vertexy << " " << vertexz
                             << " } H { " << heightx << " " << heighty << " " << heightz
                             << " } A { " << ax << " " << ay << " " << az
                             << " } B { " << bx << " " << by << " " << bz
                             << " } C { " << cx << " " << cy << " " << cz
                             << " } D { " << dx << " " << dy << " " << dz << "}\n";
    PrmContent += ss.str();
    return name;
}

string tor
(        
 string name,
 double vertexx, double vertexy, double vertexz,
 double heightx, double heighty, double heightz,
 double ra, double rh
)
{
    // put {tor.s} tor V {0 0 0}  H {0 0 1}  r_a 5 r_h 1

    stringstream ss;
    ss << "put {" << name << "} tor V { " << vertexx << " " << vertexy << " " << vertexz
                             << " } H { " << heightx << " " << heighty << " " << heightz
                             << " } r_a " << ra
                             << "  r_h " << rh << "\n";
    PrmContent += ss.str();
    return name;
}

string color(string name, int R, int G, int B)
{
    stringstream ss;
    ss << "attr set {" << name << "} {color} {" << R << "/" << G << "/" << B << "}\n";
    PrmContent += ss.str();
    return name;
}

string attrManual(string name, string attrValue)
{
    stringstream ss;
    ss << "attr set {" << name << "}" << attrValue << "}\n";
    PrmContent += ss.str();
    return name;
}

void Manual(string line)
{
    PrmContent += line + "\n";
}

void saveASC(string path)
{
    ofstream fs(path);
    fs << PrmContent;
    fs.close();
}

string combinationsManual(string name, bool region, string tree)
{
    stringstream ss;
    ss << "put {" << name << "} comb region " << (region ? "yes" : "no") << " tree " << tree << "\n";
    PrmContent += ss.str();
    return name;
}
#if 0

#include "primitives.h"

string generateSomeRandomName()
{
    return string("gsrn") + to_string(gsrnCount++);
}

string title(string value)
{
    PrmContent += "title {" + value + "}\n";
    return "";
}

string units(string value)
{
    PrmContent += "units " + value + "\n";
    return "";
}

string arb8
(
 string name,
 double x1, double y1, double z1,
 double x2, double y2, double z2,
 double x3, double y3, double z3,
 double x4, double y4, double z4,
 double x5, double y5, double z5,
 double x6, double y6, double z6,
 double x7, double y7, double z7,
 double x8, double y8, double z8
)
{
    stringstream ss;
    ss << "put {" << name << "} arb8 V1 {" << x1 << " " << y1 << " " << z1
                              << " } V2 {" << x2 << " " << y2 << " " << z2
                              << " } V3 {" << x3 << " " << y3 << " " << z3
                              << " } V4 {" << x4 << " " << y4 << " " << z4
                              << " } V5 {" << x5 << " " << y5 << " " << z5
                              << " } V6 {" << x6 << " " << y6 << " " << z6
                              << " } V7 {" << x7 << " " << y7 << " " << z7
                              << " } V8 {" << x8 << " " << y8 << " " << z8 << "}\n";
    PrmContent += ss.str();
    return name;
}

string color(string name, int R, int G, int B)
{
    stringstream ss;
    ss << "attr set {" << name << "} {color} {" << R << "/" << G << "/" << B << "}\n";
    PrmContent += ss.str();
    return name;
}
string attrManual(string name, string attrValue)
{
    stringstream ss;
    ss << "attr set {" << name << "} " << attrValue << "\n";
    PrmContent += ss.str();
    return name;
}

void Manual(string line)
{
    PrmContent += line + "\n";
}

void saveASC(string path)
{
    ofstream fs(path);
    fs << PrmContent;
    fs.close();
}
#endif

#endif //BRLCAD_SCRIPT_PRIMITIVES
