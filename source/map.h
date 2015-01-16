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

#ifndef   MAP_H
#define   MAP_H

#include <allegro.h>
#include <vector>
#include "globals.h"
#include "error.h"
#include "xml.h"

class MapController {
     protected:
          int Width, Height;
          BITMAP * Map;
          BITMAP * MiniMap;
          BITMAP * MiniMapMask;
          ErrorHandler * Error;
          std::vector <std::vector <int> > Passable;
     public:
          MapController();
          ~MapController();
          void LoadMap(const char * Filename);
          void MakeMinimap();
          void Render(BITMAP * Buffer, Point Actual);
          void MapProcessXML(IlluvienXMLTag Tag);
          Point GetSize();
          bool IsPassable(Point Loc, int Status);
};
extern MapController * Map;
#endif
