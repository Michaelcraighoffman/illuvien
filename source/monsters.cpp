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

#include "monsters.h"

MonsterController * MonsterManager;

/*
     (constructor)MonsterController()
     Sets up the error handler and loads all the monsters into memory
*/
MonsterController::MonsterController() {
     Error=new ErrorHandler("Monsters", ERROR_SEVERITY_LOG, false);
     Tooltip = nullptr;
     DisplayTooltip=false;
     ProcessXML();
}

/*
     (destructor)~MonsterController()
     Removes the error handler
*/
MonsterController::~MonsterController() {
     delete Error;
}
/*
     AddMonster(unsigned int ID, Point Position, int Ally)
     Adds a monster by it's ID value at given Position with the given Allegiance
*/
void MonsterController::AddMonster(unsigned int ID, Point Position, int Ally) {
     if(ID < MonsterLibrary.size()) {
          Monster Out=MonsterLibrary[ID];
          Out.Position=Position;
          Out.Facing=ANIM_IDLE;
          Out.Ally=Ally;
          Out.Spawn=Position;
          LoadImages(Out);
          AddMonster(Out);
     } else {
          Error->ReportError(ERROR_SEVERITY_WARN, "Could not load monster by ID: "+IntToString(ID));
     }
}
/*
     AddMonster(Monster In)
     Adds the monster given to the internal table
*/
void MonsterController::AddMonster(Monster In) {
     /* Sanity Check Later */
     In.GUID=RandomNumber(0, 1000000);
     Monsters.push_back(In);
     ReacquireThreat();
}

/*
     AddMonster(std::string Name, Point Position, int Ally)
     Adds a monster given it's Name.  Slower than via ID
     Returns true on success, false on failure
*/
bool MonsterController::AddMonster(std::string Name, Point Position, int Ally) {
     for(unsigned int i=0; i<MonsterLibrary.size(); i++) {
          if(MonsterLibrary[i].Battle.Name==Name) {
               AddMonster(i, Position, Ally);
               return true;
          }
     }
     Error->ReportError(ERROR_SEVERITY_WARN, "Could not load monster by name: "+Name);
     return false;
}
/*
     AddMonster(std::string SearchName, std::string NewName, Point Position, int Ally)
     Locates a monster by name, then renames it and adds it to the listing.
     Used for giving generic guards city names like "Synen Guard"
     Returns true on success, false on failure
*/
bool MonsterController::AddMonster(std::string SearchName, std::string NewName, Point Position, int Ally) {
     for(unsigned int i=0; i<MonsterLibrary.size(); i++) {
          if(MonsterLibrary[i].Battle.Name==SearchName) {
               /* We're going to temporarily poison the Monster Library */
               std::string OldName=MonsterLibrary[i].Battle.Name; /* Store the original name */
               MonsterLibrary[i].Battle.Name=NewName;
               AddMonster(i, Position, Ally);
               MonsterLibrary[i].Battle.Name=OldName; /* Then restore the old name */
               return true;
          }
     }
     Error->ReportError(ERROR_SEVERITY_WARN, "Could not load monster by name: "+SearchName);
     return false;
}

/*
     LoadImages(Monster &Out)
     Loads all the images associated with the given monster and adds an animator for them.
     Usally performed after one of the AddMonster variants.
*/
void MonsterController::LoadImages(Monster &Out) {
     Options * Opts=new Options;
     SDL_Surface * Image;
     std::string Path=Opts->ModDirectory+"/images/monsters/"+Out.Graphic;
     SDL_Surface * Data=IMG_Load(Path.c_str());
     if(!Data) {
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load image: "+Out.Graphic);
     }
     Out.Size.x=Data->w/4;
     Out.Size.y = Data->h / NUM_ANIMATIONS;
     std::array<Animation, NUM_ANIMATIONS> AnimHold;
     SDL_Rect src;
     src.w = Data->w / 4;
     src.h = Data->h / NUM_ANIMATIONS;
     Image = SDL_CreateRGBSurface(0, Data->w / 4, Data->h / NUM_ANIMATIONS, 32, 0, 0, 0, 0);
     SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 0, 255));
     for (int i = 0; i<NUM_ANIMATIONS; i++) {

         if (!Image)
             Error->ReportError(ERROR_SEVERITY_FATAL, "Could not create Bitmap");
         src.x = 0;
         src.y = i*(Data->h / NUM_ANIMATIONS);
         SDL_BlitSurface(Data, &src, Image, nullptr);
         AnimHold[i].Frames[0] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, nullptr);
         AnimHold[i].Frames[1] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[2] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[3] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, nullptr);
         AnimHold[i].Frames[4] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[5] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[6] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         src.x += (Data->w / 4);
         SDL_BlitSurface(Data, &src, Image, nullptr);
         AnimHold[i].Frames[7] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[8] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
         AnimHold[i].Frames[9] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
     }
     SDL_FreeSurface(Image);
     Out.AnimatorIndex=Anim->AddAnimator(ANIM_FULL, Out.MovementSpeed, AnimHold, Out.Spawn);
     SDL_FreeSurface(Data);
     delete Opts;
}

/*
     MoveMonsters()
     Cycles through the all the living monsters and calls their appropriate handlers
*/
void MonsterController::MoveMonsters(int Delta) {
     for(unsigned int i=0; i<Monsters.size(); i++) {
  
          if(Monsters[i].Battle.MeleeCooldown > 0)
               Monsters[i].Battle.MeleeCooldown-=Delta;
          if (Monsters[i].MovementCooldown > 0)
              Monsters[i].MovementCooldown -= Delta;
          switch(Monsters[i].Behavior) {
               case BEHAVIOR_GUARD:
                    MonsterAIGuard(i);
                    break;
               case BEHAVIOR_WANDER:
                    MonsterAIWander(i);
                    break;
               case BEHAVIOR_PURSUE:
                    MonsterAIPursue(i);
                    break;
          }
     }
     CheckDead();
}

/*
     BITMAP * Render(BITMAP * Buffer, Point Actual)
     Cycles through the all monsters and renders them to the screen
*/
void MonsterController::Render(Point Actual) {
     int XPos, YPos;
     Point Position;
     for(unsigned int i=0; i<Monsters.size(); i++) {
          Position=Anim->GetPosition(Monsters[i].AnimatorIndex);
          if(  Position.x >= (Actual.x-64)
               && Position.x < (Actual.x+768+64)
               && Position.y >= (Actual.y-64)
               && Position.y < (Actual.y+640+64)) {
                    XPos=(Position.x-Actual.x)-(Monsters[i].Size.x-32);
                    YPos=(Position.y-Actual.y)-(Monsters[i].Size.y-32);
                    SDL_Rect dest;  
                    dest.x = XPos;
                    dest.y = YPos;
                    dest.w = Monsters[i].Size.x;
                    dest.h = 5;
                    SDL_SetRenderDrawColor(DefaultRenderer, 0, 0, 0, 0);
                    SDL_RenderFillRect(DefaultRenderer, &dest);

                    if(Monsters[i].Ally==ALLY_FRIENDLY)
                        SDL_SetRenderDrawColor(DefaultRenderer, 0, 255, 0, 0);
                    else if(Monsters[i].Ally==ALLY_NEUTRAL)
                        SDL_SetRenderDrawColor(DefaultRenderer, 255,255, 0, 0);
                    else if(Monsters[i].Ally==ALLY_HOSTILE)
                        SDL_SetRenderDrawColor(DefaultRenderer, 255, 0, 0, 0);;
                    dest.w = (int)(((float)Monsters[i].Battle.Health / Monsters[i].Battle.MaxHealth)*Monsters[i].Size.x);
                    SDL_RenderFillRect(DefaultRenderer, &dest);
          }

     }
     if (DisplayTooltip){
         SDL_Rect dest;
         dest.x = TooltipCoords.x;
         dest.y = TooltipCoords.y;
         dest.w = 300;
         dest.h = 30;
         SDL_RenderCopy(DefaultRenderer, Tooltip, nullptr, &dest);
     }
}

/*
     SwitchPositions(int a, int b)
     A simple function that switches the two given monsters
*/
void MonsterController::SwitchPositions(int a, int b) {
     Monster Hold;
     Hold=Monsters[a];
     Monsters[a]=Monsters[b];
     Monsters[b]=Hold;
}
/*
     DamageMonster(int Which, int Amt, char Type)
     Damages the monster "Which" by "Amt" damage of type "Type"
*/
void MonsterController::DamageMonster(int Which, int Amt, char Type) {
     Monsters[Which].Battle.Health -= Amt;
     if(Amt!=0)
          Anim->AddDamageAnimator(Amt, Monsters[Which].Position);
}
/*
     CheckDead()
     Cycles through all the monsters and verifies if they are dead
*/
void MonsterController::CheckDead() {
     for(unsigned int i=0; i<Monsters.size(); i++) {
          if(Monsters[i].Battle.Health <= 0) {
               RemoveFromThreatTables(i);
               Anim->RemoveAnimatorGUID(Monsters[i].AnimatorIndex);
               Monsters.erase(Monsters.begin()+i);
               ReacquireThreat();
          }
     }
}

int MonsterController::MonsterAt(Point At) {
     for(unsigned int i=0; i<Monsters.size(); i++) {
          if(Monsters[i].Position==At) {
               return i;
          }
     }
     return -1;
}

/*
     CreateTooltip(int x, int y)
     Creates a tooltip of information about the monster at x,y
*/
void MonsterController::CreateTooltip(int x, int y, int offsetx, int offsety) {
     int Mob=-1;
     if((Mob=MonsterAt(Point(x/32,y/32)))!=-1) {
         SDL_DestroyTexture(Tooltip);
         SDL_Surface * placeHolder = SDL_CreateRGBSurface(0, 300, 30, 32, 0, 0, 0, 0);
         SDL_FillRect(placeHolder, nullptr, SDL_MapRGB(placeHolder->format, 128, 128, 128));
          //textprintf_ex(Tooltip, font, 5,5, makecol(0,0,0), -1, "%s", Monsters[Mob].Battle.Name.c_str());
          //textprintf_ex(Tooltip, font, 5,15, makecol(0,0,0), -1, "%i / %i Hitpoints", Monsters[Mob].Battle.Health, Monsters[Mob].Battle.MaxHealth);
         Tooltip = SDL_CreateTextureFromSurface(DefaultRenderer, placeHolder);
          TooltipCoords.x=x-offsetx;
          TooltipCoords.y=y-offsety;
          DisplayTooltip=true;
     }
     else
          DisplayTooltip=false;
}

