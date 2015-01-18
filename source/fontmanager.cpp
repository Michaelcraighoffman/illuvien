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
#include "fontmanager.h"
#include <string>
#include "options.h"

TTF_Font * FontManager::Interface;
TTF_Font * FontManager::Overlay;

void FontManager::LoadFonts() {
    std::string path = Options::ModDirectory + "/fonts/Vera.ttf";
    Interface = TTF_OpenFont(path.c_str(), 12);
    path = Options::ModDirectory + "/fonts/VeraBd.ttf";
    Overlay = TTF_OpenFont(path.c_str(), 12); 
}

TTF_Font * FontManager::GetInterfaceFont() {
    return Interface;
}

TTF_Font * FontManager::GetOverlayFont() {
    return Overlay;
}
