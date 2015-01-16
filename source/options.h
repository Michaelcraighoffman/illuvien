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

#ifndef   OPTIONS_H
#define   OPTIONS_H

#include "xml.h"
#include <string>

#define   COMBAT_NONE    0    /* Do not display ANY combat messages */
#define   COMBAT_LITE    1    /* Displays only special attacks, deaths, and summons */
#define   COMBAT_NORMAL  2    /* Additionally displays damage done to all parties */
#define   COMBAT_VERBOSE 3    /* Additionally displays the amount of damage evaded, damage types, etc */

class Options {
     public:
          static bool UseFullscreen;         /* Whether or not we are allowed to use a fullscreen window */
          static bool UseVsync;              /* Whether or not we are allowed to use vsync */
          static bool UseVRAM;               /* Whether or not we should try video memory */
          static bool UseDirectx;            /* Whether or not we should try to use DirectX video modes */
          static int Framerate;              /* The desired framerate, 0 for unlimitied */
          static int CurrentMode;            /* The current graphics mode we are in */
          static int CombatVerbose;          /* The verbosity of combat messages */
		static int IconCache;			/* The number of item icons to keep in memory before deleting */
          static std::string ModDirectory;   /* The current directory to pull files out of */
          Options();
          Options(bool);
          ~Options();
          void ProcessTag(IlluvienXMLTag Tag);
          void SaveOptions();
};

#endif
