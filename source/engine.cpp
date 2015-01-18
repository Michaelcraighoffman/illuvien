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
#include "fontmanager.h"
#include <iostream>

/* The number of milliseconds to wait between rendering frames */
int FrameDelay;

/* The number of milliseconds since the last frame we drew */
int LastFrame;

GameEngine::GameEngine(SDL_Window * activeWindow) {
     Map=new MapController;
     Error=new ErrorHandler("Engine", ERROR_SEVERITY_LOG, false);
     Player=new PlayerController;
     Anim=new AnimatorController;
     FPS=new FPSTimer;
     MonsterManager=new MonsterController;
     MsgBox=new IlluvienMessageBox;
     Opts=new Options;
     FontManager::LoadFonts();
    Window=activeWindow;
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
     Map->LoadMap("synen");
     GameDone=false;
     Player->Setup();
    if(Opts->Framerate > 0) {
        FrameDelay=1000/Opts->Framerate;
    }
    else {
        FrameDelay=0;
    }
     
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
    int delta=1;
    double performanceFrequencyMS = SDL_GetPerformanceFrequency()/1000.0f;
    Uint64 performanceLast = SDL_GetPerformanceCounter();
    while(!GameDone) {
        //SDL_Delay can take up to 15ms to return because of scheduling
        //Use the performance counter to avoid the program running too fast
        //and making delta=0
        while ((SDL_GetPerformanceCounter() - performanceLast) / performanceFrequencyMS < 1.0f) { }
        delta = (int)((SDL_GetPerformanceCounter() - performanceLast) / performanceFrequencyMS);
        performanceLast = SDL_GetPerformanceCounter();
        Action(delta);
        Anim->Update(delta);
        Render(delta);
        InputLoop(delta);
        if (delta = 0) { Error->ReportError(ERROR_SEVERITY_LOG, "Got 0 delta"); }
     }
}
void GameEngine::Render(int delta) {
     if(FrameDelay>0) {
        LastFrame+=delta;
        if(LastFrame<FrameDelay) return;
        else LastFrame-=FrameDelay;
     }
     FPS->Frame();
     SDL_RenderClear(DefaultRenderer);
     Actual=GetRenderRect();
     Map->Render(Actual);
     Anim->Render(Actual);
     MonsterManager->Render(Actual);
     MsgBox->Render();
     SDL_Surface * fpsSurface = TTF_RenderText_Solid(FontManager::GetOverlayFont(), IntToString(FPS->GetFPS()).c_str(), { 255, 255, 255 });
     SDL_Texture * fpsTexture = SDL_CreateTextureFromSurface(DefaultRenderer, fpsSurface);
     SDL_RenderCopy(DefaultRenderer, fpsTexture, NULL, CreateSDLRect(0, 0, fpsSurface->w, fpsSurface->h).get());
     //textprintf_ex(Buffer, font, 0,0, makecol(0,0,0), -1, "%i", FPS->GetFPS());
     //textprintf_ex(Buffer, font, 0,10, makecol(0,0,0), -1, "%i,%i", mouse_x, mouse_y);
     SDL_RenderPresent(DefaultRenderer);
     SDL_FreeSurface(fpsSurface);
     SDL_DestroyTexture(fpsTexture);
}

void GameEngine::InputLoop(int Delta) {
    SDL_Event event;
    if(SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_MOUSEMOTION:
                if(event.motion.x < 768 && event.motion.y < 640) {
                    MonsterManager->CreateTooltip(event.motion.x+Actual.x, event.motion.y+Actual.y, Actual.x, Actual.y);
                }
                break;
            default:
                break;
        }
    }
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    Point NewPosition = Player->GetPosition();
    if (state[SDL_SCANCODE_LEFT]) {
        NewPosition.x--;
        if (Map->IsPassable(NewPosition, Player->GetMountStatus())) {
            Player->SetDestination(NewPosition, ANIM_WEST);
        }
        else {
            NewPosition.x++;
        }
    }
    else if (state[SDL_SCANCODE_RIGHT]) {
        NewPosition.x++;
        if (Map->IsPassable(NewPosition, Player->GetMountStatus())) {
            Player->SetDestination(NewPosition, ANIM_EAST);
        }
        else {
            NewPosition.x--;
        }

    }
    else if (state[SDL_SCANCODE_UP]) {
        NewPosition.y--;
        if (Map->IsPassable(NewPosition, Player->GetMountStatus())) {
            Player->SetDestination(NewPosition, ANIM_NORTH);
        }
        else {
            NewPosition.y++;
        }
    }
    else if (state[SDL_SCANCODE_DOWN]) {
        NewPosition.y++;
        if (Map->IsPassable(NewPosition, Player->GetMountStatus())) {
            Player->SetDestination(NewPosition, ANIM_SOUTH);
        }
        else {
            NewPosition.y--;
        }

    }

    if(state[SDL_SCANCODE_ESCAPE]) {
         GameDone=true;
    }
}

void GameEngine::Action(int delta) {
     Player->PlayerAction(delta);
     MonsterManager->MoveMonsters(delta);
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
