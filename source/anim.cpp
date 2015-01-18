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
//!  Anim.h
/*!
    Handles the animation for the player, monsters, spell effects, and anything else that changes or moves
    */

//TODO Lord this code is ugly.  Needs to be greatly simplified.

#include "anim.h"
#include "fontmanager.h"

#include <SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;
AnimatorController * Anim;

bool Animator::operator<(const Animator &B) {
    /* Once Animators > Simple Animators > full Animators */
    if (this->Type < B.Type)
        return false;
    if (this->Type > B.Type)
        return true;


    if (this->Position.y != B.Position.y)
        return this->Position.y < B.Position.y;
    /* The y-values are equal */
    if (this->Position.x != B.Position.x)
        return this->Position.x < B.Position.x;
    
    /* The Animators are on the same tile */
   
    
    return false;
}

AnimatorController::AnimatorController()
{

}

AnimatorController::~AnimatorController() {
    Animators.clear();
    RequireSort = true;
}

int AnimatorController::AddAnimator(int Type, int Duration, std::array<Animation, NUM_ANIMATIONS> Animations, Point Location) {
    Animator New;
    New.Type = Type;
    New.Action = ANIM_IDLE;
    std::copy(std::begin(Animations), std::end(Animations), std::begin(New.Animations));
    New.Position = Location * 32;
    New.Destination = Location * 32;
    New.GUID = RandomNumber(0, 1000000);
    New.CurrentFrame = 0;
    New.FrameTime = 0;
    New.LastMovement = 0;
    New.Duration = Duration;
    Animators.push_back(New);
    RequireSort = true;
    return New.GUID;
}
int AnimatorController::AddAnimatorOnce(std::array<Animation, NUM_ANIMATIONS> Animations, int Time, Point Location) {
    Animator New;
    New.Type = ANIM_ONCE;
    New.Action = ANIM_IDLE;
    std::copy(std::begin(Animations), std::end(Animations), std::begin(New.Animations));
    New.Position.x = Location.x * 32;
    New.Position.y = Location.y * 32;
    New.Destination.x = Location.x * 32;
    New.Destination.y = Location.y * 32;
    New.GUID = RandomNumber(0, 1000000);
    New.CurrentFrame = 0;
    New.Duration = Time;
    New.FrameTime = 0;
    New.LastMovement = 0;
    Animators.push_back(New);
    RequireSort = true;
    return New.GUID;
}

void AnimatorController::AddDamageAnimator(int Amount, Point Location) {
    SDL_Surface * Hold;
    if (Amount > 0)
        Hold=TTF_RenderText_Solid(FontManager::GetOverlayFont(), IntToString(Amount).c_str(), { 255, 0, 0 });
    else
        Hold=TTF_RenderText_Solid(FontManager::GetOverlayFont(), IntToString(Amount).c_str(), { 0, 255, 0 });
    std::array<Animation, NUM_ANIMATIONS> Idle;
    SDL_Texture * final = SDL_CreateTextureFromSurface(DefaultRenderer, Hold);
    for (int i = 0; i < NUM_ANIMATIONS; i++) {
        for (int j = 0; j < 10; j++) {
            Idle[i].Frames[j] = final;
        }
    }
    int GUID = AddAnimatorOnce(Idle, 500, Location);
    UpdateAnimator(GUID, Point(Location.x, Location.y - 1));
    SDL_FreeSurface(Hold);
}

Point AnimatorController::GetPosition(int GUID) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        return Animators[ID].Position;
    }
    Point Null;
    Null.x = 0; Null.y = 0;
    return Null;
}
void AnimatorController::RemoveAnimatorGUID(int GUID) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        RemoveAnimatorID(ID);
    }
}
void AnimatorController::RemoveAnimatorID(int ID) {
    for (int i = 0; i < NUM_ANIMATIONS; i++) {
        for (int j = 0; j < 10; j++) {
            SDL_DestroyTexture(Animators[ID].Animations[i].Frames[j]);
        }
    }
    Animators.erase(Animators.begin() + ID);
}
void AnimatorController::UpdateAnimator(unsigned int GUID, int Action, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        UpdateAnimator(GUID, Action, Animators[ID].Destination/32, makeIdle);
    }
}
void AnimatorController::UpdateAnimator(unsigned int GUID, Point Destination, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        UpdateAnimator(GUID, Animators[ID].Action, Destination, makeIdle);
    }
}
void AnimatorController::UpdateAnimator(unsigned int GUID, int Action, Point Destination, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        Animators[ID].Action = Action;
        Animators[ID].Destination.x = Destination.x * 32;
        Animators[ID].Destination.y = Destination.y * 32;
        Animators[ID].Idle = false;
        Animators[ID].CurrentFrame = 0;
        Animators[ID].FrameTime = 0;
        Animators[ID].LastMovement = 0;
        if (makeIdle == true) {
            Animators[ID].Idle = true;
        }
    }
}

void AnimatorController::MoveTowardsDestination(int ID) {
    int PixelTime = ((Animators[ID].Duration / 10) / 3);
    while (Animators[ID].LastMovement >= PixelTime) {
        Animators[ID].LastMovement -= PixelTime;
        if (Animators[ID].Destination.x > (Animators[ID].Position.x)) {
            Animators[ID].Position.x += 1;
            if (Animators[ID].Destination.x < (Animators[ID].Position.x))
                Animators[ID].Position.x = Animators[ID].Destination.x;
        }
        else if (Animators[ID].Destination.x < (Animators[ID].Position.x)) {
            Animators[ID].Position.x -= 1;
            if (Animators[ID].Destination.x >(Animators[ID].Position.x))
                Animators[ID].Position.x = Animators[ID].Destination.x;
        }
        if (Animators[ID].Destination.y >(Animators[ID].Position.y)) {
            Animators[ID].Position.y += 1;
            if (Animators[ID].Destination.y < (Animators[ID].Position.y))
                Animators[ID].Position.y = Animators[ID].Destination.y;
        }
        else if (Animators[ID].Destination.y < (Animators[ID].Position.y)) {
            Animators[ID].Position.y -= 1;
            if (Animators[ID].Destination.y >(Animators[ID].Position.y))
                Animators[ID].Position.y = Animators[ID].Destination.y;
        }
        RequireSort = true;
    }
}

int AnimatorController::GUIDtoID(int GUID) {
    for (unsigned int i = 0; i < Animators.size(); i++) {
        if (Animators[i].GUID == GUID) {
            return i;
        }
    }
    return -1;
}
void AnimatorController::Update(int delta) {
    if (RequireSort == true) {
        sort(Animators.begin(), Animators.end());
        RequireSort = false;
    }
    Animator current;
    for (unsigned int i = 0; i<Animators.size(); i++) {
        Animators[i].FrameTime += delta;
        Animators[i].LastMovement += delta;
        MoveTowardsDestination(i);
        Animators[i].CurrentFrame = (Animators[i].FrameTime / (Animators[i].Duration / 10));
        if (Animators[i].CurrentFrame > 9) {
            Animators[i].FrameTime -= Animators[i].Duration;
            Animators[i].CurrentFrame = 0;
            if (Animators[i].Type == ANIM_FULL) {
                Animators[i].Idle = true;
                Animators[i].Position = Animators[i].Destination;
            }
            else if (Animators[i].Type == ANIM_ONCE) {
                RemoveAnimatorID(i);
            }
        }
    }
}
void AnimatorController::Render(Point Actual) {
    int Offsetx, Offsety, Width, Height;
    Point Position;
    SDL_Rect dest;
    for (const Animator &current : Animators) {
        Position = current.Position;
        if (Position.x >= (Actual.x)
            && Position.x < (Actual.x + 768)
            && Position.y >= (Actual.y)
            && Position.y < (Actual.y + 640)){
            SDL_QueryTexture(current.Animations[0].Frames[0], nullptr, nullptr, &Width, &Height);
            Offsetx = Width - 32;
            Offsety = Height - 32;
            dest.x = ((Position.x - (Actual.x)) - Offsetx);
            dest.y = ((Position.y - (Actual.y)) - Offsety);
            dest.w = Width;
            dest.h = Height;
            if (current.Idle == true)
                SDL_RenderCopy(DefaultRenderer, current.Animations[current.Action].Frames[0], nullptr, &dest);
            else
                SDL_RenderCopy(DefaultRenderer, current.Animations[current.Action].Frames[current.CurrentFrame], nullptr, &dest);
        }
    }
}


