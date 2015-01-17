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

#include <SDL_image.h>
MapController * Map;

MapController::MapController() {
     Error=new ErrorHandler("Map", ERROR_SEVERITY_LOG, false);
     /*   Our BITMAP * are initialized to garbage.  Pre-initalize them
          to pass the LoadMap() tests
     */
     Options * Opts=new Options;
     Map = NULL;
     MiniMap = NULL;
     std::string Path="./"+Opts->ModDirectory+"/images/interface/";
     Path+="minimap.png";
     SDL_Surface * temp = IMG_Load(Path.c_str());
     SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 255, 0, 255));
     MiniMapMask=SDL_CreateTextureFromSurface(DefaultRenderer,temp);
     if(!MiniMapMask)
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not minimap mask");
     SDL_FreeSurface(temp);
}
MapController::~MapController() {
     SDL_DestroyTexture(Map);
     delete Error;
     Passable.clear();
}
void MapController::LoadMap(const char * Filename) {
     IlluvienXMLParser * Parser;
     Options * Opts=new Options;
     std::string Path=Opts->ModDirectory+"/images/maps/";
     Path+=Filename;
     Path+=".png";
     SDL_DestroyTexture(Map);
     Map = SDL_CreateTextureFromSurface(DefaultRenderer, IMG_Load(Path.c_str()));
     if(!Map)
               Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load map image");
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
    SDL_DestroyTexture(MiniMap);
    //Internals.  Don't free
    int *w=new int, *h=new int;
    SDL_QueryTexture(Map, NULL, NULL, w, h);
    SDL_Surface * MiniMapHold = SDL_CreateRGBSurface(0, *w / 8, *h / 8, 32, 0, 0, 0, 0);
    SDL_Renderer * minimapRender = SDL_CreateSoftwareRenderer(MiniMapHold);


    SDL_RenderCopy(minimapRender, Map, NULL, CreateSDLRect(0, 0, *w / 8, *h / 8));
    MiniMap = SDL_CreateTextureFromSurface(DefaultRenderer, MiniMapHold);

    SDL_DestroyRenderer(minimapRender);
    SDL_FreeSurface(MiniMapHold);
}

void MapController::Render(Point Actual) {
    SDL_RenderCopy(DefaultRenderer, Map, CreateSDLRect(Actual.x, Actual.y, 768, 640), CreateSDLRect(0, 0, 768, 640));
    SDL_RenderCopy(DefaultRenderer, Map, CreateSDLRect(Actual.x, Actual.y, 1536,1280), CreateSDLRect(800, 0, 192, 160));
    SDL_RenderCopy(DefaultRenderer, MiniMapMask, NULL, CreateSDLRect(800, 0, 192, 160));
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
