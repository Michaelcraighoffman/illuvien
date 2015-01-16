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

#include "engine.h"
#include <iostream>
volatile int InputCounter=0;
volatile int DrawCounter=0;
volatile int ActionCounter=0;
volatile int AnimatorCounter=0;
void IncrementInputCounter() {
     InputCounter++;
}
void IncrementDrawCounter() {
     DrawCounter=1;
}
void IncrementActionCounter() {
     ActionCounter++;
}
void IncrementAnimatorCounter() {
     AnimatorCounter++;
}
GameEngine::GameEngine() {
     Map=new MapController;
     Error=new ErrorHandler("Engine", ERROR_SEVERITY_LOG, false);
     Player=new PlayerController;
     Anim=new AnimatorController;
     FPS=new FPSTimer;
     MonsterManager=new MonsterController;
     MsgBox=new IlluvienMessageBox;
     Opts=new Options;
}

GameEngine::~GameEngine() {
     delete Map;
     delete Error;
     delete Player;
     delete Anim;
     delete FPS;
     delete MonsterManager;
     delete MsgBox;
}
void GameEngine::Setup() {
     LOCK_VARIABLE(InputCounter);
     LOCK_VARIABLE(DrawCounter);
     LOCK_VARIABLE(ActionCounter);
     LOCK_VARIABLE(AnimatorCounter);
     LOCK_FUNCTION(IncrementInputCounter);
     LOCK_FUNCTION(IncrementDrawCounter);
     LOCK_FUNCTION(IncrementActionCounter);
     LOCK_FUNCTION(IncrementAnimatorCounter);
     install_int_ex(IncrementInputCounter, BPS_TO_TIMER(20));
     install_int_ex(IncrementActionCounter, BPS_TO_TIMER(5));
     if(Opts->Framerate > 0) {
          install_int_ex(IncrementDrawCounter, BPS_TO_TIMER(Opts->Framerate));
          install_int_ex(IncrementAnimatorCounter, BPS_TO_TIMER(Opts->Framerate));
     }
     Map->LoadMap("synen");
     GameDone=false;
     Player->Setup();
     PlayerPosition=Player->GetPosition();

     MonsterManager->AddMonster("Immortal Zombie", "Bonegrinder", Point(20, 11), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Immortal Zombie", "Bonegrinder", Point(22, 11), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Guard", "Synen Guard", Point(19, 28), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Guard", "Synen Guard", Point(23, 28), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Guard", "Synen Guard", Point(34, 28), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Guard", "Synen Guard", Point(36, 28), ALLY_FRIENDLY);
     MonsterManager->AddMonster("Zombie", "Mindless Servant", Point(21, 11), ALLY_HOSTILE);
     MonsterManager->AddMonster("Zombie", "Mindless Servant", Point(19, 14), ALLY_HOSTILE);
     MonsterManager->AddMonster("Zombie", "Mindless Servant", Point(21, 16), ALLY_HOSTILE);
     MonsterManager->AddMonster("Zombie", "Mindless Servant", Point(21, 15), ALLY_HOSTILE);
}
void GameEngine::Loop() {
     while(!GameDone) {
         if(ActionCounter > 0) {
               Action();
               ActionCounter--;
         }
         else rest(1);
     }
}

bool GameEngine::PThreadRenderLoop() {
    if(GameDone)
	return false;
    if(DrawCounter > 0 ) {
        Render();
        DrawCounter--;
    }
    if(AnimatorCounter > 0) {
        Anim->Update();
        AnimatorCounter--;
    }
    if(Opts->Framerate==0) {
        DrawCounter=1;
        AnimatorCounter=1;
    }
    if(DrawCounter+AnimatorCounter==0)
        rest(1);

    return true;
}
bool GameEngine::PThreadInputLoop() {
     if(GameDone)
	  return false;
     if(InputCounter > 0) {
	     InputLoop();
	     InputCounter--;
     }
     else rest(1);
     return true;
}
void GameEngine::Render() {
     FPS->Frame();
     Actual=GetRenderRect();
     BITMAP * Buffer=create_bitmap(1024,768);
     if(!Buffer)
               Error->ReportError(ERROR_SEVERITY_FATAL, "Could not create Back Buffer");
     Map->Render(Buffer, Actual);
     Anim->Render(Buffer, Actual);
     MonsterManager->Render(Buffer, Actual);
     MsgBox->Render(Buffer);
     textprintf_ex(Buffer, font, 0,0, makecol(0,0,0), -1, "%i", FPS->GetFPS());
     textprintf_ex(Buffer, font, 0,10, makecol(0,0,0), -1, "%i,%i", mouse_x, mouse_y);
     if(Opts->UseVsync) vsync();
     blit(Buffer, screen, 0,0,0,0, 1024,768);
     destroy_bitmap(Buffer);
}

void GameEngine::InputLoop() {
     Point NewPosition=PlayerPosition;
     if(key[KEY_LEFT] || key[KEY_A] || key[KEY_4_PAD]) {
          NewPosition.x--;
          if(Map->IsPassable(NewPosition, Player->GetMountStatus())) {
               Player->SetDestination(NewPosition, ANIM_WEST);
          }
          else {
               NewPosition.x++;
          }
     }
     else if(key[KEY_RIGHT]|| key[KEY_D] || key[KEY_6_PAD]) {
          NewPosition.x++;
          if(Map->IsPassable(NewPosition, Player->GetMountStatus())) {
               Player->SetDestination(NewPosition, ANIM_EAST);
          }
          else {
               NewPosition.x--;
          }

     }
     else if(key[KEY_UP]|| key[KEY_W] || key[KEY_8_PAD]) {
          NewPosition.y--;
          if(Map->IsPassable(NewPosition, Player->GetMountStatus())) {
               Player->SetDestination(NewPosition, ANIM_NORTH);
          }
          else {
               NewPosition.y++;
          }
     }
     else if(key[KEY_DOWN]|| key[KEY_S] || key[KEY_2_PAD]) {
          NewPosition.y++;
          if(Map->IsPassable(NewPosition, Player->GetMountStatus())) {
               Player->SetDestination(NewPosition, ANIM_SOUTH);
          }
          else {
               NewPosition.y--;
          }

     }
     else if(key[KEY_ESC])
          GameDone=true;

     if(mouse_x < 768 && mouse_y < 640) {
           MonsterManager->CreateTooltip(mouse_x+Actual.x, mouse_y+Actual.y, Actual.x, Actual.y);
     }
}

void GameEngine::Action() {
     Player->PlayerAction();
     MonsterManager->MoveMonsters();
     PlayerPosition=Player->GetPosition();
}

Point GameEngine::GetRenderRect() {
     Point Actual;
     Point Position=Player->GetPixelPosition();
     Point Size=Map->GetSize();
     if(Position.x-384 < 0)
          Actual.x=0;
     else if(Position.x+352 > Size.x*32)
          Actual.x=(Size.x*32)-736;
     else
          Actual.x=Position.x-384;
     if(Position.y-320 < 0)
          Actual.y=0;
     else if(Position.y+288 > Size.y*32)
          Actual.y=(Size.y*32)-608;
     else
          Actual.y=Position.y-320;
     return Actual;
}
