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

AnimatorController::AnimatorController()
{
}

AnimatorController::~AnimatorController() {
    Animators.clear();
}


bool AnimatorController::AddAnimator(Animator * anim) {
    Animators.push_back(anim);
    return true;
}

void AnimatorController::AddDamageAnimator(int Amount, Point Location) {
    Animator * anim = new Animator();
    anim->CreateDamageAnimator(Amount, Location);
    Animators.push_back(anim);
}



void AnimatorController::Update(int delta) {
    auto currentAnimator = Animators.begin();
    for (; currentAnimator != Animators.end(); currentAnimator++) {
        (*currentAnimator)->Update(delta);
    }
    //Find all the elements which are finished and ready to be removed
    auto firstErased = std::remove_if(Animators.begin(), Animators.end(), [](Animator * a)->bool { if (a->IsFinished()) { delete a; return true; } return false; });
    //for (; firstErased != Animators.end();) {
    //    //Free the memory associated with them
    //    delete (*firstErased);
    //    firstErased = Animators.erase(firstErased);
    //}
    //And finally remove them
    Animators.erase(firstErased, Animators.end());
}
void AnimatorController::Render(Point Actual) {
    std::sort(Animators.begin(), Animators.end(), [](Animator *a, Animator *b) { return *a < *b; });
    for (auto current=Animators.begin(); current != Animators.end(); current++) {
        (*current)->Render(Actual);
    }
}


