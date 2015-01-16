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

#include "loadpng.h"
MapController * Map;

MapController::MapController() {
     Error=new ErrorHandler("Map", ERROR_SEVERITY_LOG, false);
     /*   Our BITMAP * are initialized to garbage.  Pre-initalize them
          to pass the LoadMap() tests
     */
     Map=create_bitmap(1,1);
     MiniMap=create_bitmap(1,1);
     Options * Opts=new Options;
     std::string Path=Opts->ModDirectory+"/images/interface/";
     Path+="minimap.png";
     MiniMapMask=load_png(Path.c_str(), NULL);
     if(!MiniMapMask)
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not minimap mask");
}
MapController::~MapController() {
     destroy_bitmap(Map);
     delete Error;
     Passable.clear();
}
void MapController::LoadMap(const char * Filename) {
     IlluvienXMLParser * Parser;
     Options * Opts=new Options;
     std::string Path=Opts->ModDirectory+"/images/maps/";
     Path+=Filename;
     Path+=".png";
     BITMAP * Data=load_png(Path.c_str(), NULL);
     if(!Data) {
          std::string Err="Could not load image: ";
          Err+=Filename;
          Error->ReportError(ERROR_SEVERITY_FATAL, Err);
     }
     if(Map)
          destroy_bitmap(Map);
     Map=create_display_bitmap(Data->w, Data->h);
     if(!Map)
               Error->ReportError(ERROR_SEVERITY_FATAL, "Could not create Video Bitmap");
     blit(Data, Map, 0,0,0,0,Data->w, Data->h);
     destroy_bitmap(Data);
     Path=Opts->ModDirectory+"/maps/";
     Path+=Filename;
     Path+=".map";
     Parser=new IlluvienXMLParser(Path);
     if(Parser->CheckFile() == false)
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load map XML data");
     else {
          Parser->Parse();
          IlluvienXMLTag * Head=Parser->GetHead();
          for(unsigned int i=0; i<Head->Children.size(); i++)
               MapProcessXML(*Head->Children[i]);
     }
     delete Parser;
     delete Opts;
     MakeMinimap();
}
void MapController::MakeMinimap() {
     if(MiniMap)
          destroy_bitmap(MiniMap);
     BITMAP * MiniMapHold=create_bitmap(Map->w/8, Map->h/8);
     MiniMap=create_bitmap(Map->w/8, Map->h/8);
     /* We need to put the map in a memory bitmap for scaling */
     BITMAP * MapHold=create_bitmap(Map->w, Map->h);
     blit(Map, MapHold, 0,0,0,0,Map->w, Map->h);
     stretch_blit(MapHold, MiniMapHold, 0,0,Map->w, Map->h,0,0,Map->w/8, Map->h/8);
     blit(MiniMapHold, MiniMap, 0,0,0,0,MiniMap->w, MiniMap->h);
     destroy_bitmap(MapHold);
     destroy_bitmap(MiniMapHold);
}

void MapController::Render(BITMAP * Buffer, Point Actual) {
     blit(Map, Buffer, Actual.x, Actual.y, 0,0,768,640);
     blit(MiniMap, Buffer, Actual.x/8, Actual.y/8, 800,0,192, 160);
     masked_blit(MiniMapMask, Buffer, 0,0,800,0, 192, 160);
     //rectfill(Buffer, 898, 82, 902, 86, makecol(255,255,255));
}
Point MapController::GetSize() {
     Point Out;
     Out.x=Width;
     Out.y=Height;
     return Out;
}
bool MapController::IsPassable(Point Loc, int Status) {
     return (Passable[Loc.x][Loc.y] >= Status);
}
