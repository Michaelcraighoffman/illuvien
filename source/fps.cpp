/*
    Illuvien: A single player, turn-based RPG
    Copyright (C) 2007  Michael Hoffman

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

#include "fps.h"

FPSTimer::FPSTimer()
{
     Start=time(NULL);
     FPSThisFrame=0;
}
int FPSTimer::GetFPS()
{
     return FPS;
}
void FPSTimer::Frame()
{
     Last=time(NULL);
     FPSThisFrame++;
     if(Last-Start >= 1)
     {
          FPS=FPSThisFrame;
          FPSThisFrame=0;
          Start=time(NULL);
     }
}

