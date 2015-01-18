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

#ifndef   MESSAGEBOX_H
#define   MESSAGEBOX_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "globals.h"

/* Constants to make AddMessage input a bit more readable */
#define   MSGBOXRED      "^255000000"
#define   MSGBOXGREEN    "^000255000"
#define   MSGBOXBLUE     "^000000255"
#define   MSGBOXBLACK    "^000000000"
#define   MSGBOXWHITE    "^255255255"
#define   MSGBOXYELLOW   "^255255000"
#define   MSGBOXORANGE   "^255128000"

//! A single piece of text used in the messagebox
struct TextPiece {
     /*! The Actual text */
     std::string Text;
     /*! The color to print this portion of the message in */
     SDL_Color Color;
};
//! A messagebox message
struct Message {
     /*! Vector of individual colored pieces of text */
     std::vector <TextPiece> Pieces;
};
//! Controller for the ingame Message Box
class IlluvienMessageBox {
     protected:
          /*! Vector of all the reported messages */
          std::vector <Message> Messages;
          /*! Background image for the messagebox */
          SDL_Texture * MessageBoxImage;   

          /*! Pre-Rendering of the messagebox */
          SDL_Surface * SurfacePrerender;
          /*! Pre-Rendering of the messagebox */ 
          SDL_Texture * Prerender;
     public:
          IlluvienMessageBox();
          ~IlluvienMessageBox();
          /*! Converts a string to a message structure and adds it to the queue */
          void AddMessage(std::string Msg);
          /*! Adds the message to the queue */
          void AddMessage(Message Msg);
          /*! Pre-Renders the Messagebox */
          void ReRender();
          /*! Renders the messagebox to the Buffer */
          void Render();
          /*! Renders a single string to the buffer */
          void RenderString(SDL_Surface * Buffer, int Which);
};
extern IlluvienMessageBox * MsgBox;
#endif
