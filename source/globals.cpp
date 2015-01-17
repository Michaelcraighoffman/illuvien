#include "globals.h"

SDL_Renderer * DefaultRenderer;
void PopupBox(std::string msg) {
     PopupBox(msg.c_str());
}

void PopupBox(const char * msg) {
          /* Fake later */
}

int StringToInt(std::string in) {
     std::stringstream buffer(in);
	int Ret;
	buffer >> Ret;
	return Ret;
}

std::string IntToString(int Value, int Base) {
     enum { kMaxDigits = 35 };
	std::string buf;
	buf.reserve( kMaxDigits ); // Pre-allocate enough space.
	// check that the base if valid
	if (Base < 2 || Base > 16) return buf;
	int quotient = Value;
	// Translating number to string with base:
	do {
		buf += "0123456789abcdef"[ std::abs( quotient % Base ) ];
		quotient /= Base;
	} while ( quotient );
     // Append the negative sign for base 10
	if ( Value < 0 && Base == 10) buf += '-';
	std::reverse( buf.begin(), buf.end() );
	return buf;
}
std::string IntToString(int Value) {
     return IntToString(Value, 10);
}

int RandomNumber(int Low, int High) {
     /* If we set the parameters backwards, switch them */
     if(Low > High) {
          int Hold=Low;
          Low=High;
          High=Hold;
     }
     return rand() % (High - Low + 1) + Low;
}

bool Distance(Point A, Point B, int Dist) {
     if(  ( ((A.x-B.x)*(A.x-B.x)) + ((A.y-B.y)*(A.y-B.y)) ) <= (Dist*Dist) ) return true;
     return false;
}

std::string BooltoString(bool In) {
     if(In==true)
          return "true";
     return "false";
}

SDL_Rect * CreateSDLRect(int x, int y, int w, int h){
    SDL_Rect * rect = new SDL_Rect();
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
    return rect;
}
