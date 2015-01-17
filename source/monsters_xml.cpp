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
#include <iostream>
void MonsterController::ProcessXML() {
     Options * Opts=new Options;
     std::string Path=Opts->ModDirectory+"/data/monsters.xml";
     IlluvienXMLParser * Parser;
     Parser=new IlluvienXMLParser(Path.c_str());
     if(!Parser->CheckFile()) {
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load monsters.xml!");
     }
     else {
          Parser->Parse();
          Monster * Current;          
          IlluvienXMLTag * Head=Parser->GetHead();
          for(unsigned int i=0; i<Head->Children.size(); i++) {
               Current=new Monster;        
               for(unsigned int j=0; j<Head->Children[i]->Children.size(); j++) {
                    ProcessTag(*Head->Children[i]->Children[j], Current);
               }
               MonsterLibrary.push_back(*Current);
               delete Current;
          }                    
     }
     Error->ReportError(ERROR_SEVERITY_LOG, "Processed "+IntToString(MonsterLibrary.size())+" monsters.");
     delete Parser;
     delete Opts;
}

void MonsterController::ProcessTag(IlluvienXMLTag Tag, Monster * Current) {
     if(Tag.Name == "name") {
          Current->Battle.Name=Tag.Data;
     }
     else if(Tag.Name == "image") {
          Current->Graphic=Tag.Data;
     }
     else if(Tag.Name == "behavior") {
          Current->Behavior=ResolveBehavior(Tag.Data);
     }
     else if(Tag.Name == "aggrorange") {
          Current->AggroDistance=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "meleerange") {
          Current->Battle.MeleeRange=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "leashdistance") {
          Current->LeashDistance=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "level") {
          Current->Battle.Level=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "strength") {
          Current->Battle.SkillStrength=StringToInt(Tag.Data);
          Current->Battle.SkillStrengthMod=0;
     }
     else if(Tag.Name == "perception") {
          Current->Battle.SkillPerception=StringToInt(Tag.Data);
          Current->Battle.SkillPerceptionMod=0;
     }
     else if(Tag.Name == "attunement") {
          Current->Battle.SkillAttunement=StringToInt(Tag.Data);
          Current->Battle.SkillAttunementMod=0;
     }
     else if(Tag.Name == "evasion") {
          Current->Battle.SkillEvasion=StringToInt(Tag.Data);
          Current->Battle.SkillEvasionMod=0;
     }
     else if(Tag.Name == "absorbance") {
          Current->Battle.SkillAbsorbance=StringToInt(Tag.Data);
          Current->Battle.SkillAbsorbanceMod=0;
     }
     else if(Tag.Name == "stealth") {
          Current->Battle.SkillStealth=StringToInt(Tag.Data);
          Current->Battle.SkillStealthMod=0;
     }
     else if(Tag.Name == "armor") {
          Current->Battle.Armor=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "maxhitpoints") {
          Current->Battle.MaxHealth=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "hitpoints") {
          Current->Battle.Health=StringToInt(Tag.Data);
     }
     else if(Tag.Name == "meleespeed") {
          Current->Battle.MeleeSpeed=StringToInt(Tag.Data);
          Current->Battle.MeleeCooldown=0;
     }
     else {
          Error->ReportError(ERROR_SEVERITY_WARN, "Parsed Unknown Tag: "+Tag.Name+" with data: "+Tag.Data);
     }
}

int MonsterController::ResolveBehavior(std::string Behavior) {
     if(Behavior == "guard")
          return BEHAVIOR_GUARD;
     if(Behavior == "pursue")
          return BEHAVIOR_PURSUE;
     if(Behavior == "wander")
          return BEHAVIOR_WANDER;
     /* Unknown behavior, report and use default behavior */
     else
          Error->ReportError(ERROR_SEVERITY_WARN, "Monster had unknown behavior: "+Behavior);
          return BEHAVIOR_PURSUE;
}


