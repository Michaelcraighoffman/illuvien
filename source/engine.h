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

//!  Engine.h
/*!
     Handles the rendering and input
*/

#ifndef   ENGINE_H
#define   ENGINE_H

#include "SDL.h"

#include "anim.h"
#include "map.h"
#include "player.h"
#include "fps.h"
#include "monsters.h"
#include "messagebox.h"

//! The game engine
/*! Singleton handling Rendering and input for the entire game */
class GameEngine {
     protected:
          /*! Errorhandler for Errors in the engine */
          ErrorHandler * Error;

          /*! Class to measure FPS */
          FPSTimer * FPS;

          /*! Last recorded position of mouse, for performance */
          Point LastMouse;

          /*! Point containing the top left coordinate of the drawn screen.
              Enables the screen to be drawn such that the player is centered
              and transformation of mouse coordinates to game coordinates */ 
          Point Actual;

        /*! Boolean recording whether some action has occured such that the game
            needs to be shut down gracefully*/ 
        bool GameDone;

        /*! Local Options Object */
        Options * Opts;

        /*! The SDL Window we're drawing to */
        SDL_Window * Window;

        /*! The renderer for the SDL Window */
        SDL_Renderer * Renderer;
     public:
          GameEngine(SDL_Window * activeWindow);
          ~GameEngine();
          /*! Sets up portions of the engine not suitable for the constructor */
          void Setup();

          /*! Controls the timing for player and NPC actions */
          void Loop();


          /*! Renders the entire screen.  Called from ThreadRenderLoop() */          
          void Render(int delta);

          /*! Checks for input and routes events accordingly */
          void InputLoop();

          /*! Updates player and casues NPC to process actions */
          void Action(int delta);

          /*! Returns the top left coordinate to begin drawing the screen.
              Enables the screen to be drawn such that the player is centered */ 
          Point GetRenderRect();
};

#endif
