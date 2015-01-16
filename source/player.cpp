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

PlayerController * Player;

PlayerController::PlayerController() {
     Error=new ErrorHandler("Player", ERROR_SEVERITY_LOG, false);
}

PlayerController::~PlayerController() {
     delete Error;
}

void PlayerController::Setup() {
     Options * Opts=new Options;
     BITMAP * Image;
     Position.x=21;
     Position.y=11;
     Destination=Position;
     std::string Path=Opts->ModDirectory+"/images/player/priest/priest.bmp";
     BITMAP * Data=load_bitmap(Path.c_str(), NULL);
     if(!Data) {
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load image: Priest");
     }
     Animation AnimHold[7];
     for(int i=0; i<7; i++) {
          Image=create_display_bitmap(Data->w/4, Data->h/7);
          if(!Image)
               Error->ReportError(ERROR_SEVERITY_FATAL, "Could not create Bitmap");
          blit(Data, Image, 0,i*(Data->h/7),0,0,Data->w/4, Data->h/7);
          AnimHold[i].Frames[0]=get_rle_sprite(Image);
          blit(Data, Image, 32,i*(Data->h/7),0,0,Data->w/4, Data->h/7);
          AnimHold[i].Frames[1]=get_rle_sprite(Image);
          AnimHold[i].Frames[2]=get_rle_sprite(Image);
          AnimHold[i].Frames[3]=get_rle_sprite(Image);
          blit(Data, Image, 64,i*(Data->h/7),0,0,Data->w/4, Data->h/7);
          AnimHold[i].Frames[4]=get_rle_sprite(Image);
          AnimHold[i].Frames[5]=get_rle_sprite(Image);
          AnimHold[i].Frames[6]=get_rle_sprite(Image);
          blit(Data, Image, 96,i*(Data->h/7),0,0,Data->w/4, Data->h/7);
          AnimHold[i].Frames[7]=get_rle_sprite(Image);
          AnimHold[i].Frames[8]=get_rle_sprite(Image);
          AnimHold[i].Frames[9]=get_rle_sprite(Image);
          destroy_bitmap(Image);
     }
     AnimationIndex=Anim->AddAnimator(ANIM_FULL, AnimHold, Position);
     destroy_bitmap(Data);
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
void PlayerController::PlayerAction() {
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
