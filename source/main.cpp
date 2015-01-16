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
#include <allegro.h>
#ifdef _WIN32
#include <winalleg.h>
#endif
#include <pthread.h>

#include "options.h"
#include "error.h"
#include "engine.h"

GameEngine * Engine;  //Global for pthread

bool SetupGraphics(Options opt) {
     set_color_depth(opt.ColorDepth);
     //If the player will allow fullscreen
     ErrorHandler Error("GFXINIT", ERROR_SEVERITY_LOG, false);
     if(opt.UseFullscreen==true)
     {
		//If supported, try DirectX
		#ifdef GFX_DIRECTX
		if(opt.UseDirectx==true) {
            if (set_gfx_mode(GFX_DIRECTX, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_DIRECTX; return true; }
            Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Fullscreen DirectX Window");
		}
		#endif
		//If supported, try Linux Hardware acceleration
		#ifdef GFX_XDGA2
		if (set_gfx_mode(GFX_XDGA2, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_XDGA2; return true; }
        Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Fullscreen XDGA Window");
		#endif
		//Try to create any fullscreen window
        if (set_gfx_mode(GFX_AUTODETECT_FULLSCREEN, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_AUTODETECT_FULLSCREEN; return true; }
        Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Fullscreen Generic Window");
     }
    //If supported, try the DirectX overlay driver
    #ifdef GFX_DIRECTX_OVL
    if (opt.UseDirectx==true) {
        if (set_gfx_mode(GFX_DIRECTX_OVL, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_DIRECTX; return true; }
        Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Windowed DirectX Overlay Window");
    }
    #endif
	//How about generic Windowed DirectX
	#ifdef GFX_DIRECTX_WIN
	if(opt.UseDirectx==true) {
        if (set_gfx_mode(GFX_DIRECTX_WIN, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_DIRECTX_WIN; return true; }
        Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Windowed DirectX Window");
	}
	#endif
     //Try to autodetect for any windowed screen
     if (set_gfx_mode(GFX_AUTODETECT_WINDOWED, 1024, 768, 0, 0) == 0) { opt.CurrentMode=GFX_AUTODETECT_WINDOWED; return true; }
     Error.ReportError(ERROR_SEVERITY_LOG, "Could not create Generic Window");
     //We can't initialize the window
	return false;
}
void * PThreadRenderRedirect(void * unused) {
    while(Engine->PThreadRenderLoop()) { }
    pthread_exit(NULL);
}
void * PThreadInputRedirect(void * unused) {
    while(Engine->PThreadInputLoop()) { }
    pthread_exit(NULL);
}
int main(int Argc, char * Argv[]) {
     allegro_init();
     install_timer();
     install_keyboard();
     install_mouse();
     Options opts(true);

     pthread_t RenderThread;
     pthread_t InputThread;
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


     if(SetupGraphics(opts)) {
          Engine=new GameEngine;
          enable_hardware_cursor();
          show_mouse(screen);
          Engine->Setup();
          pthread_create(&RenderThread, NULL, PThreadRenderRedirect,NULL);
          pthread_create(&InputThread, NULL, PThreadInputRedirect, NULL);
          Engine->Loop();
	  //Once the engine completes, wait for our render thread to do so before shutting down
	  pthread_join(RenderThread, NULL);
	  pthread_join(InputThread, NULL);
          delete Engine;
          return EXIT_SUCCESS;
     }
     return EXIT_FAILURE;
}
END_OF_MAIN()
