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

/* ****************************** */
/* Measures our frames per second */
/* ****************************** */

#ifndef   FPS_H
#define   FPS_H

#include <time.h>

//! Structure for meausring FPS
class FPSTimer
{
     protected:
          /*! FPS from the last second */
          int FPS;
          /*! Counter of FPS for this second */
          int FPSThisFrame;
          /*! The start time of the counter */
          time_t Start;
          /*! One second past the start time */
          time_t Last;
     public:
          FPSTimer();
          /*! Returns the FPS from the previous second */
          int GetFPS();
          /*! Counts a single drawn frame.  
              Also resets timer and stores FPS if timer has expired */
          void Frame();
};
#endif
