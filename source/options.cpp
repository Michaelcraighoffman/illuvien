/*
    Illuvien: A single player, turn-based RPG
    Copyright (C) 2008  Michael Hoffman

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globals.h"
#include "options.h"
#include "xml.h"
#include <fstream>
/* The Static Definitions */
bool Options::UseFullscreen;
bool Options::UseVsync;
bool Options::UseVRAM;
bool Options::UseDirectx;
int  Options::Framerate;
int  Options::CurrentMode;
int  Options::CombatVerbose;
int  Options::IconCache;
std::string Options::ModDirectory;
Options::Options() {

}

Options::Options(bool reset) {
     if(reset==true) {
          UseFullscreen=false;
          UseVsync=false;
          UseVRAM=true;
          UseDirectx=true;
          Framerate=60;
          CombatVerbose=COMBAT_VERBOSE;
		IconCache=100;
          ModDirectory="default";

          IlluvienXMLParser * Parser;
          Parser=new IlluvienXMLParser("options.cfg");
          if(!Parser->CheckFile()) {
               SaveOptions();
          }
          else {
               Parser->Parse();
               IlluvienXMLTag * Head=Parser->GetHead();
               for(unsigned int i=0; i<Head->Children.size(); i++)
                    ProcessTag(*Head->Children[i]);
          }
          delete Parser;
     }
}
Options::~Options() {
    SaveOptions();
}
void Options::ProcessTag(IlluvienXMLTag Tag) {
     if(Tag.Name=="usefullscreen") {
          UseFullscreen=false;
          if(Tag.Data=="true")
               UseFullscreen=true;
     }
     else if(Tag.Name=="usevsync") {
          UseVsync=false;
          if(Tag.Data=="true")
               UseVsync=true;
     }
     else if(Tag.Name=="usevram") {
          UseVRAM=true;
          if(Tag.Data=="false")
               UseVRAM=false;
     }
     else if(Tag.Name=="usedirectx") {
         UseDirectx=false;
         if(Tag.Data=="true")
            UseDirectx=true;
     }
     else if(Tag.Name=="framerate") {
          Framerate=StringToInt(Tag.Data);
     }
     else if(Tag.Name=="combatverbose") {
          CombatVerbose=StringToInt(Tag.Data);
     }
	else if(Tag.Name=="iconcache") {
		IconCache=StringToInt(Tag.Data);
	}
     else if(Tag.Name=="moddirectory") {
          ModDirectory=Tag.Data;
     }
}

void Options::SaveOptions() {
     std::ofstream File("options.cfg", std::ios::trunc);
     File<<"<options>\n";
     File<<"    <usefullscreen>";
     File<<BooltoString(UseFullscreen);
     File<<"</usefullscreen>\n";
     File<<"    <usevsync>";
     File<<BooltoString(UseVsync);
     File<<"</usevsync>\n";
     File<<"    <usevram>";
     File<<BooltoString(UseVRAM);
     File<<"</usevram>\n";
     File<<"    <usedirectx>";
     File<<BooltoString(UseDirectx);
     File<<"</usedirectx>\n";
     File<<"    <framerate>";
     File<<Framerate;
     File<<"</framerate>\n";
     File<<"    <combatverbose>";
     File<<CombatVerbose;
     File<<"</combatverbose>\n";
	File<<"    <iconcache>";
	File<<IconCache;
	File<<"</iconcache>\n";
     File<<"    <moddirectory>";
     File<<ModDirectory;
     File<<"</moddirectory>\n";
     File<<"</options>";
     File.close();
}
