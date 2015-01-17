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

//TODO:
//Lord this code is ugly.  Needs to be greatly simplified.

#include "anim.h"
#include <iostream>
#include <fstream>
using namespace std;
AnimatorController * Anim;


//! The time, in MS, since we last updated the animator
int AnimationTicks = 0;

//! The number of milliseconds between Animator ticks
#define ANIMATION_TICK_MS 25

bool AnimatorController::CompareAnimators(int A, int B) {
    bool AisSimple = false, BisSimple = false;
    if (Animators[A].Type == ANIM_ONCE || Animators[A].Type == ANIM_SIMPLE)
        AisSimple = true;
    if (Animators[B].Type == ANIM_ONCE || Animators[B].Type == ANIM_SIMPLE)
        BisSimple = true;
    if (AisSimple == true && BisSimple == false)
        return false;
    if (AisSimple == false && BisSimple == true)
        return true;

    if (Animators[A].Position.y < Animators[B].Position.y)
        return true;
    if (Animators[A].Position.y > Animators[B].Position.y)
        return false;
    /* The y-values are equal */
    if (Animators[A].Position.x < Animators[B].Position.x)
        return true;
    return false;
}

static bool Compare(int A, int B) {
    return Anim->CompareAnimators(A, B);
}

AnimatorController::AnimatorController()
{
    AnimationTicks = 0;
}

AnimatorController::~AnimatorController() {
    Animators.clear();
    RequireSort = true;
}

int AnimatorController::AddAnimator(int Type, Animation Animations[7], Point Location) {
    Animator New;
    New.Type = Type;
    New.Action = ANIM_IDLE;
    for (int i = 0; i < 7; i++) {
        New.Animations[i] = Animations[i];
    }
    New.Position.x = Location.x * 32;
    New.Position.y = Location.y * 32;
    New.Destination.x = Location.x * 32;
    New.Destination.y = Location.y * 32;
    New.GUID = RandomNumber(0, 1000000);
    New.CurrentFrame = 0;
    Animators.push_back(New);
    RenderList.push_back(Animators.size() - 1);
    return New.GUID;
}
int AnimatorController::AddAnimatorOnce(Animation Animations[7], int Frames, Point Location) {
    Animator New;
    New.Type = ANIM_ONCE;
    New.Action = ANIM_IDLE;
    for (int i = 0; i < 7; i++) {
        New.Animations[i] = Animations[i];
    }
    New.Position.x = Location.x * 32;
    New.Position.y = Location.y * 32;
    New.Destination.x = Location.x * 32;
    New.Destination.y = Location.y * 32;
    New.GUID = RandomNumber(0, 1000000);
    New.CurrentFrame = 0;
    New.SimpleEnd = Frames;
    Animators.push_back(New);
    RenderList.push_back(Animators.size() - 1);
    return New.GUID;
}

void AnimatorController::AddDamageAnimator(int Amount, Point Location) {
    ///TODO: implement damage animator
#ifdef NOTIMPLEMENTED
    BITMAP * Hold = create_bitmap(30, 10);
    clear_to_color(Hold, makecol(255, 0, 255));
    if (Amount > 0)
        textprintf_ex(Hold, font, 0, 0, makecol(255, 0, 0), -1, "%i", Amount);
    else
        textprintf_ex(Hold, font, 0, 0, makecol(0, 255, 0), -1, "%i", Amount);
    Animation Idle[7];
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 10; j++) {
            Idle[i].Frames[j] = get_rle_sprite(Hold);
        }
    }
    int GUID = AddAnimatorOnce(Idle, 80, Location);
    UpdateAnimator(GUID, Point(Location.x, Location.y - 2));
    destroy_bitmap(Hold);
#endif
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
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 10; j++) {
            SDL_DestroyTexture(Animators[ID].Animations[i].Frames[j]);
        }
    }
    Animators.erase(Animators.begin() + ID);
    Resort();
}
void AnimatorController::UpdateAnimator(unsigned int GUID, int Action, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        Animators[ID].Action = Action;
        Animators[ID].Idle = false;
        Animators[ID].CurrentFrame = 0;
        if (makeIdle == true) {
            Animators[ID].Idle = true;
        }
    }
}
void AnimatorController::UpdateAnimator(unsigned int GUID, Point Position, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        Animators[ID].Destination.x = Position.x * 32;
        Animators[ID].Destination.y = Position.y * 32;
        Animators[ID].Idle = false;
        Animators[ID].CurrentFrame = 0;
        if (makeIdle == true) {
            Animators[ID].Idle = true;
        }
    }
}
void AnimatorController::UpdateAnimator(unsigned int GUID, int Action, Point Position, bool makeIdle) {
    int ID = GUIDtoID(GUID);
    if (ID != -1) {
        Animators[ID].Action = Action;
        Animators[ID].Destination.x = Position.x * 32;
        Animators[ID].Destination.y = Position.y * 32;
        Animators[ID].Idle = false;
        Animators[ID].CurrentFrame = 0;
        if (makeIdle == true) {
            Animators[ID].Idle = true;
        }
    }
}

void AnimatorController::MoveTowardsDestination(int ID) {
    if (Animators[ID].Destination.x > (Animators[ID].Position.x)) {
        Animators[ID].Position.x += 4;
        if (Animators[ID].Destination.x < (Animators[ID].Position.x))
            Animators[ID].Position.x = Animators[ID].Destination.x;
    }
    else if (Animators[ID].Destination.x < (Animators[ID].Position.x)) {
        Animators[ID].Position.x -= 4;
        if (Animators[ID].Destination.x >(Animators[ID].Position.x))
            Animators[ID].Position.x = Animators[ID].Destination.x;
    }
    if (Animators[ID].Destination.y >(Animators[ID].Position.y)) {
        Animators[ID].Position.y += 4;
        if (Animators[ID].Destination.y < (Animators[ID].Position.y))
            Animators[ID].Position.y = Animators[ID].Destination.y;
    }
    else if (Animators[ID].Destination.y < (Animators[ID].Position.y)) {
        Animators[ID].Position.y -= 4;
        if (Animators[ID].Destination.y >(Animators[ID].Position.y))
            Animators[ID].Position.y = Animators[ID].Destination.y;
    }
    RequireSort = true;
}

int AnimatorController::GUIDtoID(int GUID) {
    for (unsigned int i = 0; i < Animators.size(); i++) {
        if (Animators[i].GUID == GUID) {
            return i;
        }
    }
    return -1;
}
void AnimatorController::Resort() {
    for (unsigned int i = 0; i < RenderList.size(); i++) {
        if (RenderList[i] >= Animators.size())
            RenderList.erase(RenderList.begin() + i);
    }
    RequireSort = true;
}
void AnimatorController::Update(int delta) {
    if (RequireSort == true) {
        sort(RenderList.begin(), RenderList.end(), Compare);
        RequireSort = false;
    }
    AnimationTicks += delta;
    if (AnimationTicks > ANIMATION_TICK_MS) {
        for (unsigned int i = 0; i<Animators.size(); i++) {
            Animators[i].CurrentFrame ++;
            MoveTowardsDestination(i);
            if (Animators[i].CurrentFrame>9) {
                Animators[i].CurrentFrame = 0;
                if (Animators[i].Type == ANIM_FULL) {
                    Animators[i].Idle = true;
                    Animators[i].Position = Animators[i].Destination;
                }
            }
            if (Animators[i].Type == ANIM_ONCE) {
                Animators[i].SimpleEnd -= AnimationTicks;
                if (Animators[i].SimpleEnd <= 0)
                    RemoveAnimatorID(i);
            }
        }
        AnimationTicks -= ANIMATION_TICK_MS ;
    }
}
void AnimatorController::Render(Point Actual) {
    int Offsetx, Offsety, i, Width, Height;
    unsigned int size = RenderList.size();
    Point Position;
    SDL_Rect dest;
    for (unsigned int j = 0; j < size; j++) {
        i = RenderList[j];
        Position = Animators[i].Position;
        if (Position.x >= (Actual.x)
            && Position.x < (Actual.x + 768)
            && Position.y >= (Actual.y)
            && Position.y < (Actual.y + 640)){
            SDL_QueryTexture(Animators[i].Animations[0].Frames[0], NULL, NULL, &Width, &Height);
            Offsetx = Width - 32;
            Offsety = Height - 32;
            dest.x = ((Position.x - (Actual.x)) - Offsetx);
            dest.y = ((Position.y - (Actual.y)) - Offsety);
            dest.w = Width;
            dest.h = Height;
            if (Animators[i].Idle == true)
                SDL_RenderCopy(DefaultRenderer, Animators[i].Animations[Animators[i].Action].Frames[0], NULL, &dest);
            else
                SDL_RenderCopy(DefaultRenderer, Animators[i].Animations[Animators[i].Action].Frames[Animators[i].CurrentFrame], NULL, &dest);
        }
    }
}


