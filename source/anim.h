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

#ifndef   ANIM_H
#define   ANIM_H


#include <algorithm>
#include <array>
#include <set>
#include "animator.h"
#include "globals.h"

//! The Animator Controller
/*! Controls all the animators */
class AnimatorController {
     protected:
		/*! All the animators */
         std::vector <Animator *> Animators;

		/*! Whether or not some action has occured to require the Animators to be resorted before a render */
          bool RequireSort;
     public:
         AnimatorController();

           //! Generic destructor
          ~AnimatorController();
          //!  Adds an animator
          /*!  Inserts the given animator into the Animators vector. */
          bool AddAnimator(Animator * anim);
          //!  Adds an animator to show damage
          /*!  Creates a bitmap to show the amount of damage done, then calls AddAnimatorOnce to add the animator */
          void AddDamageAnimator(int Amount, Point Location);
          //!  Updates all animators
          void Update(int delta);
          //!  Renders all animators
          void Render(Point Actual);
};

extern AnimatorController * Anim;

#endif
