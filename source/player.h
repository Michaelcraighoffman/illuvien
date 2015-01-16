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

#ifndef   PLAYER_H
#define   PLAYER_H

#include <allegro.h>
#include <string>
#include "globals.h"
#include "battle.h"
#include "error.h"
#include "anim.h"

struct PlayerStatistics {
     std::string Name;
     char Class;
     int Experience;

     BattleInfo Battle;
};

class PlayerController {
     protected:
          ErrorHandler * Error;
          Point Position;
          Point Destination;
          int NewAnim;
          bool Mounted;
          int AnimationIndex;
          PlayerStatistics Statistics;
     public:
          PlayerController();
          ~PlayerController();
          Point GetPosition();
          Point GetPixelPosition() ;
          void Setup();
          void SetPosition(Point New);
          void SetAnim(int Anim);
          void SetDestination(Point To, int Anim);
          void PlayerAction();
          int GetMountStatus();

          void DamagePlayer(int Amt);
          bool CheckDead();

          void GiveExperience(int Amount);
};

extern PlayerController * Player;

#endif
