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
     Position.x=21;
     Position.y=11;
     Destination=Position;
     std::string Path=Options::ModDirectory+"/images/player/priest/priest.bmp";
     PlayerAnimator = new Animator();
     PlayerAnimator->Create(ANIM_FULL, 200, Path, Position);
     Anim->AddAnimator(PlayerAnimator);
}

Point PlayerController::GetPosition() {
     return Position;
}

Point PlayerController::GetPixelPosition() {
    return PlayerAnimator->GetPosition();
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
          PlayerAnimator->UpdateAnimator(NewAnim, Destination);
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
