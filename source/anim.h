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

#ifndef   ANIM_H
#define   ANIM_H


#include <algorithm>
#include <array>
#include <vector>
#include "globals.h"

//!  Anim.h
/*!
    Handles the animation for the player, monsters, spell effects, and anything else that changes or moves
*/

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
struct Animator {
     /*! All the animations */
    std::array<Animation, NUM_ANIMATIONS> Animations;
     int Type;
     /*! The action the animator is currently performing */
     int Action;
     /*! Position of the animator (in pixel coordinates) */
     Point Position;
     /*! The final destination of the animator if it is moving (in pixel coordinates) */
     Point Destination;
     /*! A unique identifier attached to each Animator */
     int GUID;
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

     bool operator<(const Animator &B);
};
//! The Animator Controller
/*! Controls all the animators */
class AnimatorController {
     protected:
		/*! All the animators */
          std::vector <Animator> Animators;

		/*! Whether or not some action has occured to require the Animators to be resorted before a render */
          bool RequireSort;
     public:
         AnimatorController();

           //! Generic destructor
          ~AnimatorController();
          //!  Adds an animator
          /*!  Uses the given values to prepare an Animator struct, then adds it to the Animators vector. */
          int AddAnimator(int Type, int Duration, std::array<Animation, NUM_ANIMATIONS> Animations, Point Location);
          //!  Adds an animator which is shown once
          /*!  Uses the given values to prepare an Animator struct, then adds it to the Animators vector. */
          int AddAnimatorOnce(std::array<Animation, NUM_ANIMATIONS> Animations, int Frames, Point Location);
          //!  Adds an animator to show damage
          /*!  Creates a bitmap to show the amount of damage done, then calls AddAnimatorOnce to add the animator */
          void AddDamageAnimator(int Amount, Point Location);
          //!  Returns the position of the indicated Animator.
          Point GetPosition(int GUID);
          //!  Removes an animator by GUID
          /*!  Translates the GUID to an ID, then calls RemoveAnimatorID */
          void RemoveAnimatorGUID(int GUID);
          //!  Removes an animator by ID
          void RemoveAnimatorID(int ID);
          //!  Updates an animator with a new action
          void UpdateAnimator(unsigned int ID, int Action, bool makeIdle=false);
          //!  Updates an animator with a new destination
          void UpdateAnimator(unsigned int ID, Point Position, bool makeIdle=false);
          //!  Updates an animator with both a new action and destination
          void UpdateAnimator(unsigned int ID, int Action, Point Position, bool makeIdle=false);
          //!  Helper Function to move an animator to it's destination
          void MoveTowardsDestination(int ID) ;
          //!  Helper Function to translate a GUID to it's corresponding ID
          int GUIDtoID(int GUID);
          //!  Updates all animators
          void Update(int delta);
          //!  Renders all animators
          void Render(Point Actual);
};

extern AnimatorController * Anim;

#endif
