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

#include "map.h"

void MapController::MapProcessXML(IlluvienXMLTag Tag) {
     if(Tag.Name == "width") {
          Width=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "height") {
          Height=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "passablecol") {
          std::vector <int> Column;
          std::string Out;
          for(int y=0; y<Height; y++) {
                    Out=Tag.Data[y];
                    Column.push_back(StringToInt(Out.c_str()));
          }
          Passable.push_back(Column);
     }
     else {
          Error->ReportError(ERROR_SEVERITY_WARN, "Parsed Unknown Tag: "+Tag.Name+" with data: "+Tag.Data);
     }
}

