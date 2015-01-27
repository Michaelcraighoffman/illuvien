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

#ifndef   GLOBALS_H
#define   GLOBALS_H

#include <SDL.h>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <memory>
#include "options.h"

#define   CLASS_WARRIOR 0
#define   CLASS_HUNTER  1
#define   CLASS_MAGE    2

#define   MAP_NOTPASSABLE     0
#define   MAP_MOUNTPASSABLE   1
#define   MAP_FOOTPASSABLE    2

#define   ALLY_HOSTILE        0
#define   ALLY_NEUTRAL        1
#define   ALLY_FRIENDLY       2

#define   BEHAVIOR_GUARD      0    /*   The monster will stand in one spot and follow enemies until it reaches it's leash length.*/
#define   BEHAVIOR_PURSUE     1    /*   The monster will wander until it aggros, then pursue enemies indefinitely*/
#define   BEHAVIOR_WANDER     2    /*   The monster will wander, regardless of aggro*/

extern SDL_Renderer * DefaultRenderer;
extern ErrorHandler * GlobalError;

//! Structure defining a simple x,y pair
class Point {
	public:     
		/*! x,y pair */     
     	int x,y;
		Point() { x=0;y=0; }
		Point(int inx, int iny) { x=inx; y=iny; }
		bool operator==(const Point& Right) { return (x == Right.x && y == Right.y); }
		bool operator!=(const Point& Right) { return (x != Right.x || y != Right.y); }
		bool operator>(const Point& Right)  { return (y > Right.y || (y == Right.y && x > Right.x)); }
		bool operator>=(const Point& Right) { return (y > Right.y || (y == Right.y && x >=Right.x)); }
		bool operator<(const Point& Right)  { return (y < Right.y || (y == Right.y && x < Right.x)); }
		bool operator<=(const Point& Right) { return (y < Right.y || (y == Right.y && x <=Right.x)); }
        Point operator/(const int val) { Point n; n.x = x / val; n.y = y / val; return n; }
        Point operator*(const int val) { Point n; n.x = x * val; n.y = y * val; return n; }
};

//! Structure implementing a simple reference counter for RLE_SPRITE's
struct SPRITE_REF { 
     /*! The actual sprite data */
   SDL_Texture * sprite;
     /*! Some unique identifier */
	int ID;
     /*! Reference Counter */
	int refcount;
};

/*! Redirect to PopupBox(const char *) */
void PopupBox(std::string msg);
/*! Displays a message either in a system popup box or
    a faked popup box depending on fullscreen status */ 
void PopupBox(const char * msg);

/*! Converts an std::string to an integer */
int StringToInt(std::string in);
/*! Converts an integer to a std::string */
std::string IntToString(int Value, int Base);
/*! Redirect to IntToString(int, int) using 10 as Base */
std::string IntToString(int Value);

/*! Returns a random number between Low and High */
int RandomNumber(int Low, int High);

/*! Returns whether or not the distance between A and B is 
    greater or equal to Dist */
bool Distance(Point A, Point B, int Dist);

/*! Converts a Boolean to the string "true" or "false" */
std::string BooltoString(bool In);

/*! Creates an SDL_Rect from the given coordinates */
std::unique_ptr<SDL_Rect> CreateSDLRect(int x, int y, int w, int h);
#endif
