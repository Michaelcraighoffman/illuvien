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

#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <array>
#include <string>
#include <SDL.h>
#include "globals.h"

//! Animator Type
/*! Determines what features an animator has */
#define   ANIM_ONCE      0    /*!   Static animator which goes through it animations a preset number of times 
and is deleted (spell effects) */
#define   ANIM_SIMPLE    1    /*!   Animator which does not disappear and has only one animation (flags, arrows, etc) */
#define   ANIM_FULL      2    /*!   A Full Animatior, which can move and change animations (players, monsters) */

//! Animator Actions
/*! Determines what action an animator is performing */

#define   ANIM_IDLE      0
#define   ANIM_SOUTH     1
#define   ANIM_WEST      2
#define   ANIM_EAST      3
#define   ANIM_NORTH     4
#define   ANIM_ATTACKA   5
#define   ANIM_ATTACKB   6
#define   NUM_ANIMATIONS 7

//! A single animation
struct Animation {
    SDL_Texture * Frames[10];
};
//! A single Animator
/*! Base class holding the information for one animator */
class Animator {
private:
    /*! All the animations */
    std::array<Animation, NUM_ANIMATIONS> Animations;
    int Type;
    /*! The action the animator is currently performing */
    int Action;
    /*! Position of the animator (in pixel coordinates) */
    Point Position;
    /*! The final destination of the animator if it is moving (in pixel coordinates) */
    Point Destination;
    /*! Whether or not this animator is complete and can be removed */
    bool AnimatorFinished;
    /*! The current Frame of animation the animator is in */
    int CurrentFrame;
    /*! The number of Milliseconds a animation lasts */
    int Duration;
    /*! The number of Milliseconds since the current Animation began */
    int FrameTime;
    /*! The number of Milliseconds since the animator last moved */
    int LastMovement;
    /*! Whether or not the Animator is currently idle */
    bool Idle;
    /*! Load the animations for this animator from a file */
    void LoadImages(std::string Path);
public:
    ~Animator();
    bool operator<(const Animator &B);
    void Create(int animType, int Duration, std::string Path, Point Location);
    void CreateDamageAnimator(int Amount, Point Location);
    void UpdateAnimator(int Action, bool makeIdle=false);
    void UpdateAnimator(Point Destination, bool makeIdle=false);
    void UpdateAnimator(int Action, Point Destination, bool makeIdle=false);
    Point GetPosition();
    Point GetDimensions();
    void MoveTowardsDestination();
    void Update(int delta);
    void Render(Point Actual);
    void FinishAnimation();
    bool IsFinished();
};

#endif