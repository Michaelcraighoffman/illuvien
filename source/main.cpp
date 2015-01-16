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

#include <cstdio>
#include <cstring>
#include <iostream>
#include "SDL.h"
#include "SDL_thread.h"
#include "SDL_timer.h"

#include "options.h"
#include "error.h"
#include "engine.h"

GameEngine * Engine;  //Global for threads

bool SetupGraphics(Options opt, SDL_Window *win) {
     //If the player will allow fullscreen
     ErrorHandler Error("GFXINIT", ERROR_SEVERITY_LOG, false);
     if(opt.UseFullscreen==true)
     {
		win = SDL_CreateWindow(
                "Illuvien",                
                SDL_WINDOWPOS_UNDEFINED,   
                SDL_WINDOWPOS_UNDEFINED,   
                1024,                      
                768,                       
                SDL_WINDOW_FULLSCREEN);    
        if(win != null) return true;
        Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Fullscreen DirectX Window");
    }
		
    win = SDL_CreateWindow(
               "Illuvien",                
               SDL_WINDOWPOS_UNDEFINED,   
               SDL_WINDOWPOS_UNDEFINED,   
               1024,                      
               768,                       
               0);    
    if(win != null) return true;
    Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Generic Window");
    //We can't initialize the window
    return false;
}
int ThreadRenderRedirect(void * unused) {
    while(Engine->ThreadRenderLoop()) { }
    return 0;
}
int ThreadInputRedirect(void * unused) {
    while(Engine->ThreadInputLoop()) { }
    return 0;
}
int main(int Argc, char * Argv[]) {
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);

    Options opts(true);

    SDL_Thread * RenderThread;
    SDL_Thread * InputThread;
    SDL_Window * Window;
    for(int a=0; a<Argc; a++) {
         if(strcmp(Argv[a], "-nofullscreen")==0)
              opts.UseFullscreen=false;
         if(strcmp(Argv[a], "-usefullscreen")==0)
              opts.UseFullscreen=true;
         if(strcmp(Argv[a], "-nodirectx")==0)
              opts.UseDirectx=false;
         if(strcmp(Argv[a], "-moddir")==0) {
              if(a+1<Argc)
                   opts.ModDirectory=Argv[a+1];
              else
                   PopupBox("-moddir specified with no directory!  Using default.");
         }
    }


     if(SetupGraphics(opts, Window)) {
          Engine=new GameEngine(Window);
          enable_hardware_cursor();
          show_mouse(screen);
          Engine->Setup();
        RenderThread=SDL_CreateThread(ThreadRenderRedirect, "Render Thread", (void *)NULL);
        InputThread=SDL_CreateThread(ThreadInputRedirect, "Input Thread", (void *)NULL);
          Engine->Loop();
	  //Once the engine completes, wait for our render thread to do so before shutting down
        SDL_WaitThread(RenderThread, NULL);
        SDL_WaitThread(InputThread, NULL);
          delete Engine;
        SDL_DestroyWindow(Window);
        SDL_Quit();
          return EXIT_SUCCESS;
     }
     return EXIT_FAILURE;
}
END_OF_MAIN()
