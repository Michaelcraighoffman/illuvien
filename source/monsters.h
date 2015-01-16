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

#ifndef   MONSTERS_H
#define   MONSTERS_H

#include <allegro.h>
#include <string>
#include <vector>
#include "globals.h"
#include "battle.h"
#include "anim.h"
#include "error.h"
#include "xml.h"

//! Holds the amount of threat one target has caused
struct ThreatInfo {
     /*! The vector position of this target */
     int Index;
     /*! The unique ID of this target */     
     int GUID;
     /*! The amount of threat this target has caused */
     int Threat;
};

//! Controls a single monster Entity
class Monster {
     public:
          /*! Position of the Monster */
          Point Position;
          /*! Allegiance of the monster relative to the player */
          int Ally;
          /*! How the monster behaves */
          int Behavior;
          /*! How far the monster can travel from it's spawn point before 
              resetting aggro and returning to it */
          int LeashDistance;
          /*! The position the monster spawned at */
          Point Spawn;
          /*! The amount of experience the monster gives when killed */
          int Experience;
          /*! The amount of Gold the monster gives when killed */
          int Gold;
          
          /*! Structure containing the Battle information for this Monster */
          BattleInfo Battle;

          /*! The filename of this monster's images */
          std::string Graphic;
          /*! The index of this monster's animator */
          int AnimatorIndex;
          /*! The direction the monster is currently facing */
          int Facing;
          /*! The size of the monster (in pixels) */
          Point Size;
          /*! The aggro table of the monster */
          std::vector <ThreatInfo> ThreatTable;
          /*! How close a monster must be to gain aggro */
          int AggroDistance;
          /*! Unique Identifier */
          int GUID;
};

//! Controls the actions of all monsters
class MonsterController {
     protected:
          /*! All the Monsters present in monsters.xml */
          std::vector <Monster> MonsterLibrary;
          /*! The refcount for the images in MonsterLibrary */
          std::vector <int> MonsterLibraryRefCount;
          /*! All active Monsters */
          std::vector <Monster> Monsters;
          /*! The current Tooltip */
          BITMAP * Tooltip;
          /*! The location of the tooltip */
          Point TooltipCoords;
          /*! Whether or the tooltip should be rendered */
          bool DisplayTooltip;
          /*! Error Handler for Monsters */
          ErrorHandler * Error;
     public:
          MonsterController();
          ~MonsterController();
          /*! Adds a monster by it's ID value at given Position with the given Allegiance */
          void AddMonster(unsigned int ID, Point Position, int Ally);
          /*! Adds the monster given to the internal table */
          void AddMonster(Monster In);
          /*!  Adds a monster given it's Name.  Slower than via ID
               Returns true on success, false on failure */
          bool AddMonster(std::string Name, Point Position, int Ally);
          /*! AddMonster(std::string SearchName, std::string NewName, Point Position, int Ally)
              Locates a monster by name, then renames it and adds it to the listing.
              Used for giving generic guards city names like "Synen Guard"
              Returns true on success, false on failure */
          bool AddMonster(std::string SearchName, std::string NewName, Point Position, int Ally);
          /*! Loads all the images associated with the given monster and adds an animator for them. */
          void LoadImages(Monster &Out);
          /*! Cycles through the all the living monsters and calls their appropriate handlers */
          void MoveMonsters();
          /*! Cycles through the all monsters and renders them to the screen */
          void Render(BITMAP * Buffer, Point At);
          /*! Helper function that switches the two given monsters in the vector */
          void SwitchPositions(int a, int b);
          /*! Damages the specified monster by the amount specified, which damage of the type specified */
          void DamageMonster(int Which, int Amt, char Type);
          /*! Cycles through the monsters and removes those that have died */
          void CheckDead();
          /*! Returns the ID of the monster at the point specified, -1 if no monster exists there */
          int MonsterAt(Point At);
         
          /*! Creates a tooltip of information about the monster at x,y */
          void CreateTooltip(int x, int y, int offsetx, int offsety);
          
          /*! Loads the monsters in monsters.xml */
          void ProcessXML();
          /*! Processes an individual xml-tag */
          void ProcessTag(IlluvienXMLTag Tag, Monster * Current);
          /*! Processes a string into its corresponding Behavior constant */
          int ResolveBehavior(std::string Behavior);

          /*! Function run to control monsters with BEHAVIOR_GUARD BEHAVIOR */
          void MonsterAIGuard(int i);
          /*! Function run to control monsters with BEHAVIOR_WANDER BEHAVIOR */
          void MonsterAIWander(int i);
          /*! Function run to control monsters with BEHAVIOR_PURSUE BEHAVIOR */
          void MonsterAIPursue(int i) ;
          /*! Moves a monster towards the destination */
          void MoveTowardPosition(int i, Point Destination);
          /*! Handles one Monster attacking another */
          void Attack(int i);
          /*! Causes the specified monster to wander randomly */
          void Wander(int i);
          /*! Examines the whole monster table for monsters which have threat on one another or the player */
          void CheckThreat();
          /*! Iterates through the monster table and removes the specified monster from aggro tables */
          void RemoveFromThreatTables(int Which);
          /*! Reacquires indicies after a monster is removed */
          void ReacquireThreat();
          /*! Adds the specified amount of threat to the target from the source */
          void AddThreat(int Target, int Source, int Amt);
          /*! Causes the given monster to face a different direction */
          void AdjustFacing(int Which, Point Target);
};

extern MonsterController * MonsterManager;

#endif
