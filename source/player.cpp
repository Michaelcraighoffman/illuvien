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

#include "player.h"

#include <SDL_image.h>

#define ENGINE_TICK_MS 200

PlayerController * Player;

/* Milliseconds since the last time we updated the Player */
int LastAction;

PlayerController::PlayerController() {
     Error=new ErrorHandler("Player", ERROR_SEVERITY_LOG, false);
}

PlayerController::~PlayerController() {
     delete Error;
}

void PlayerController::Setup() {
     Options * Opts=new Options;
     SDL_Surface * Image;
     Position.x=21;
     Position.y=11;
     Destination=Position;
     std::string Path=Opts->ModDirectory+"/images/player/priest/priest.bmp";
     SDL_Surface * Data = IMG_Load(Path.c_str());
     if(!Data) {
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load image: Priest");
     }
     Animation AnimHold[7];
     SDL_Rect src;
     src.w = Data->w / 4;
     src.h = Data->h / 7;
     Image = SDL_CreateRGBSurface(0, Data->w / 4, Data->h / 7, 32, 0, 0, 0, 0);
     SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 0, 255));
     for(int i=0; i<7; i++) {
         
         if (!Image)
             Error->ReportError(ERROR_SEVERITY_FATAL, "Could not create Bitmap");
         src.x = 0;
         src.y = i*(Data->h / 7);
         SDL_BlitSurface(Data, &src, Image, NULL);
         AnimHold[i].Frames[0] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, NULL);
         AnimHold[i].Frames[1] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[2] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[3] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, NULL);
         AnimHold[i].Frames[4] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[5] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[6] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, NULL);
         AnimHold[i].Frames[7] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[8] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[9] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
     }
     SDL_FreeSurface(Image);
     AnimationIndex=Anim->AddAnimator(ANIM_FULL, 200, AnimHold, Position);
     SDL_FreeSurface(Data);
     delete Opts;
}

Point PlayerController::GetPosition() {
     return Position;
}
Point PlayerController::GetPixelPosition() {
     return Anim->GetPosition(AnimationIndex);
}

void PlayerController::SetPosition(Point New) {
     Position=New;
     Anim->UpdateAnimator(AnimationIndex, New);
}
void PlayerController::SetAnim(int New) {
     Anim->UpdateAnimator(AnimationIndex, New);
}
void PlayerController::SetDestination(Point To, int Anim) {
     Destination=To;
     NewAnim=Anim;
}
void PlayerController::PlayerAction(int delta) {
    LastAction+=delta;
    if(LastAction<ENGINE_TICK_MS) { return; }
    LastAction-=ENGINE_TICK_MS;

    if(Position!=Destination) {
          Anim->UpdateAnimator(AnimationIndex, NewAnim, Destination);
          Position=Destination;
     }
}

int PlayerController::GetMountStatus() {
     if (Mounted)
          return MAP_MOUNTPASSABLE;
     return MAP_FOOTPASSABLE;
}

void PlayerController::GiveExperience(int Amount) {
     Statistics.Experience+=Amount;
}
