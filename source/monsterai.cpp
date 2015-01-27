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
#include "map.h"
#include "player.h"

/*
     MonsterAIGuard(int i)
     Controls the AI for monsters with Guard Behavior
*/
void MonsterController::MonsterAIGuard(int i) {
     CheckThreat();
     if(Monsters[i].ThreatTable.size() > 0) {
          Attack(i);
     }
     else {
          MoveTowardPosition(i, Monsters[i].Spawn);
     }
     if(Monsters[i].Position==Monsters[i].Spawn) {
          Monsters[i].MonsterAnimator->UpdateAnimator(ANIM_SOUTH, true);
     }
}

/*
     MonsterAIWander(int i)
     Controls the AI for monsters with Wander Behavior
*/
void MonsterController::MonsterAIWander(int i) {
     Wander(i);
}
/*
     MonsterAIPursue(int i)
     Controls the AI for monsters with Pursue Behavior
*/
void MonsterController::MonsterAIPursue(int i) {
     CheckThreat();
     if(Monsters[i].ThreatTable.size() > 0) {
          Attack(i);
     }
     else {
          Wander(i);
     }
};

/*
     MoveTowardPosition(int i, Point Destination)
     Moves the Monster specified in i towards the point Destination, checking
     to ensure it does not violate leash distance, or walk on unpassable tiles
*/
void MonsterController::MoveTowardPosition(int i, Point Destination) {
   
    if (Monsters[i].MovementCooldown > 0) 
        return; //We can't move yet
     Point NewPosition=Monsters[i].Position;
     if(Destination.y > Monsters[i].Position.y) {
          NewPosition.y++;
     }
     else if(Destination.y < Monsters[i].Position.y) {
          NewPosition.y--;
     }
     if(  (abs(NewPosition.y-Monsters[i].Spawn.y) > Monsters[i].LeashDistance) ||
          !Map->IsPassable(NewPosition, MAP_FOOTPASSABLE) ||
          (MonsterAt(NewPosition) != -1) ||
          (NewPosition==Player->GetPosition())) {
               NewPosition.y=Monsters[i].Position.y;
     }
     if(NewPosition.y == Monsters[i].Position.y) {
          if(Destination.x > Monsters[i].Position.x) {
               NewPosition.x++;
          }
          else if(Destination.x < Monsters[i].Position.x) {
               NewPosition.x--;
          }
          if(  (abs(NewPosition.x-Monsters[i].Spawn.x) > Monsters[i].LeashDistance) ||
               !Map->IsPassable(NewPosition, MAP_FOOTPASSABLE) ||
               (MonsterAt(NewPosition) != -1) ||
               (NewPosition==Player->GetPosition())) {
                    NewPosition.x=Monsters[i].Position.x;
          }
     }
     if((Monsters[i].Position!=NewPosition)) {
         Monsters[i].MovementCooldown = Monsters[i].MovementSpeed;
          AdjustFacing(i, NewPosition);
          Monsters[i].Position=NewPosition;
          Monsters[i].MonsterAnimator->UpdateAnimator(NewPosition);
     }
     
}


/*
    Attack(int i)
    Checks for and attacks any monsters within range, or moves towards them
*/
void MonsterController::Attack(int i) {


     int Target=Monsters[i].ThreatTable[0].Index;
     if(Target > -1) {
          if(  abs(Monsters[i].Position.x-Monsters[Target].Position.x) <= Monsters[i].Battle.MeleeRange &&
               abs(Monsters[i].Position.y-Monsters[Target].Position.y) <= Monsters[i].Battle.MeleeRange) {
                if(Monsters[i].Battle.MeleeCooldown <= 0) {
                    AdjustFacing(i, Monsters[Target].Position);
                    Monsters[i].Battle.MeleeCooldown=Monsters[i].Battle.MeleeSpeed;
                    BattleManager BattleObject;
                    BattleResult Res=BattleObject.Battle(Monsters[i].Battle, Monsters[Target].Battle, DAMAGE_SOURCE_MELEE);
                    DamageMonster(i, Res.AttackerDamage, Res.AttackerDamageType);
                    DamageMonster(Target, Res.DefenderDamage, Res.DefenderDamageType);
                    AddThreat(i, Target, Res.AttackerDamage);
                    AddThreat(Target, i, Res.DefenderDamage);
                }
          }
          else if(  abs(Monsters[i].Spawn.x-Monsters[Target].Position.x) > Monsters[i].LeashDistance ||
                    abs(Monsters[i].Spawn.y-Monsters[Target].Position.y) > Monsters[i].LeashDistance) {
                         MsgBox->AddMessage(MSGBOXRED+Monsters[i].Battle.Name+MSGBOXBLACK+" has lost interest in "+Monsters[Monsters[i].ThreatTable[0].Index].Battle.Name+".");
                         Monsters[i].ThreatTable.erase(Monsters[i].ThreatTable.begin());
          }
          else {
               MoveTowardPosition(i, Monsters[Target].Position);
          }
     }
     else {    /* We've aggroed the player */
          Point PlayerPosition=Player->GetPosition();
          if(  abs(Monsters[i].Spawn.x-PlayerPosition.x) > Monsters[i].LeashDistance ||
               abs(Monsters[i].Spawn.y-PlayerPosition.y) > Monsters[i].LeashDistance) {
                    MsgBox->AddMessage(MSGBOXRED+Monsters[i].Battle.Name+MSGBOXBLACK+" has lost interest in Player.");
                    Monsters[i].ThreatTable.erase(Monsters[i].ThreatTable.begin());
          }
          else if(  abs(Monsters[i].Position.x-PlayerPosition.x) <= Monsters[i].Battle.MeleeRange &&
                    abs(Monsters[i].Position.y-PlayerPosition.y) <= Monsters[i].Battle.MeleeRange) {
                          if(Monsters[i].Battle.MeleeCooldown <= 0) {
                              Monsters[i].Battle.MeleeCooldown=Monsters[i].Battle.MeleeSpeed;
                              MsgBox->AddMessage("Attacked Player");
                          }
          }
          else
               MoveTowardPosition(i, PlayerPosition);
     }
}

/*
     Wander(int i)
     Moves the monster in a random direction
*/
void MonsterController::Wander(int i) {
     int Direction=RandomNumber(1,4);
     Point NewPosition=Monsters[i].Position;
     switch(Direction) {
          case ANIM_SOUTH:
               NewPosition.y++;
               break;
          case ANIM_NORTH:
               NewPosition.y--;
               break;
          case ANIM_EAST:
               NewPosition.x++;
               break;
          case ANIM_WEST:
               NewPosition.x--;
               break;
          default:
               Error->ReportError(ERROR_SEVERITY_WARN, "Got invalid wander direction: "+IntToString(Direction));
               break;
     }
     if(  (abs(NewPosition.x-Monsters[i].Spawn.x) < Monsters[i].LeashDistance &&
          abs(NewPosition.y-Monsters[i].Spawn.y) < Monsters[i].LeashDistance) &&
          Map->IsPassable(NewPosition, MAP_FOOTPASSABLE) && MonsterAt(NewPosition)==-1) {
               Monsters[i].Position=NewPosition;
               Monsters[i].MonsterAnimator->UpdateAnimator(Direction, NewPosition);
     }
}

/*
     CheckThreat()
     Examines the whole monster table for monsters which have threat on one another or the player
*/
void MonsterController::CheckThreat() {
     bool Skip=false; /* Whether we should skip adding this monster
                         to aggro table because it's already there */
     ThreatInfo ThreatTemp;   /* A temporary holder for threat info */
     ThreatTemp.Threat=10;
     for(unsigned int A=0; A<Monsters.size(); A++) {
          /* Check the player first */
          if(Monsters[A].Ally == ALLY_HOSTILE) {
               if(Distance(Monsters[A].Position, Player->GetPosition(), Monsters[A].AggroDistance)){
                    for(unsigned int i=0; i<Monsters[A].ThreatTable.size(); i++) {
                         if(Monsters[A].ThreatTable[i].Index==-1) Skip=true;
                    }
                    if(Skip==false) {
                         ThreatTemp.Index=-1;
                         ThreatTemp.GUID=0;
                         Monsters[A].ThreatTable.push_back(ThreatTemp);
                         MsgBox->AddMessage(MSGBOXRED+Monsters[A].Battle.Name+MSGBOXBLACK+" is angered by the player.");
                    }
               }
          }
          for(unsigned int B=A+1; B<Monsters.size(); B++) {
               if(  Monsters[A].Ally != ALLY_NEUTRAL &&
                    Monsters[B].Ally != ALLY_NEUTRAL &&
                    Monsters[A].Ally != Monsters[B].Ally) {
                         Skip=false;
                         if(Distance(Monsters[A].Position, Monsters[B].Position, Monsters[A].AggroDistance)) {
                              for(unsigned int i=0; i<Monsters[A].ThreatTable.size(); i++) {
                                   if(Monsters[A].ThreatTable[i].Index==(int)B) Skip=true;
                              }
                              if(Skip==false) {
                                   ThreatTemp.Index=B;
                                   ThreatTemp.GUID=Monsters[B].GUID;
                                   Monsters[A].ThreatTable.push_back(ThreatTemp);
                                   MsgBox->AddMessage(MSGBOXRED+Monsters[B].Battle.Name+MSGBOXBLACK+" has angered "+MSGBOXGREEN+Monsters[A].Battle.Name);
                              }
                              Skip=false;
                         }
                         if(Distance(Monsters[A].Position, Monsters[B].Position, Monsters[B].AggroDistance)) {
                              for(unsigned int i=0; i<Monsters[B].ThreatTable.size(); i++) {
                                   if(Monsters[B].ThreatTable[i].Index==(int)A) Skip=true;
                              }
                              if(Skip==false) {
                                   ThreatTemp.Index=A;
                                   ThreatTemp.GUID=Monsters[A].GUID;
                                   Monsters[B].ThreatTable.push_back(ThreatTemp);
                                   MsgBox->AddMessage(MSGBOXRED+Monsters[B].Battle.Name+MSGBOXBLACK+" has angered "+MSGBOXGREEN+Monsters[A].Battle.Name);
                              }
                         }
               }
          }
     }
}

void MonsterController::RemoveFromThreatTables(int Which) {
     for(unsigned int i=0; i<Monsters.size(); i++) {
          for(unsigned int t=0; t<Monsters[i].ThreatTable.size(); t++) {
               if(Monsters[i].ThreatTable[t].Index==Which) {
                    Monsters[i].ThreatTable.erase(Monsters[i].ThreatTable.begin()+t);
                    t=Monsters[i].ThreatTable.size(); /* Skip any remaining aggroed mobs */
               }
          }
     }
}

void MonsterController::ReacquireThreat() {
     int CurrentMonster=-1;
     for(unsigned int i=0; i<Monsters.size(); i++) {
          for(unsigned int t=0; t<Monsters[i].ThreatTable.size(); t++) {
               CurrentMonster=Monsters[i].ThreatTable[t].Index;
               /* First, ensure this isn't the player's aggro position */
               if(CurrentMonster != -1) {
                    for(unsigned int n=0; n<Monsters.size(); n++) {
                         if(Monsters[n].GUID==Monsters[i].ThreatTable[t].GUID) {
                              Monsters[i].ThreatTable[t].Index=n;
                              Monsters[i].ThreatTable[t].GUID=Monsters[n].GUID;
                              n=Monsters.size();
                         }
                    }
               }
          }
     }
}

void MonsterController::AddThreat(int Target, int Source, int Amt) {
     bool Found=false;
     for(unsigned int i=0; i<Monsters[Target].ThreatTable.size(); i++) {
          if(Monsters[Target].ThreatTable[i].Index==Source) {
               Monsters[Target].ThreatTable[i].Threat+=Amt;
               Found=true;
               i=Monsters[Target].ThreatTable.size();
          }
     }
     if(Found==false) {
          ThreatInfo ThreatTemp;
          ThreatTemp.Threat=Amt;
          ThreatTemp.Index=Source;
          Monsters[Target].ThreatTable.push_back(ThreatTemp);
     }
     ThreatInfo Hold;
     for(unsigned int i=1; i<Monsters[Target].ThreatTable.size(); i++) {
          if(Monsters[Target].ThreatTable[i].Threat > Monsters[Target].ThreatTable[i-1].Threat) {
               Hold=Monsters[Target].ThreatTable[i];
               Monsters[Target].ThreatTable[i]=Monsters[Target].ThreatTable[i-1];
               Monsters[Target].ThreatTable[i-1]=Hold;
               i=1;
          }
     }
}

void MonsterController::AdjustFacing(int Which, Point Target) {
     int Xdist=Monsters[Which].Position.x - Target.x;
     int Ydist=Monsters[Which].Position.y - Target.y;
     if(abs(Xdist) > abs(Ydist)) {
          if(Xdist < 0) {
               if(Monsters[Which].Facing!=ANIM_EAST) {
                   Monsters[Which].MonsterAnimator->UpdateAnimator(ANIM_EAST, true);
                    Monsters[Which].Facing=ANIM_EAST;
               }
          }
          else {
               if(Monsters[Which].Facing !=ANIM_WEST) {
                   Monsters[Which].MonsterAnimator->UpdateAnimator(ANIM_WEST, true);
                    Monsters[Which].Facing=ANIM_WEST;
               }
          }
     }
     else if(abs(Xdist) < abs(Ydist)){
          if(Ydist < 0) {
               if(Monsters[Which].Facing != ANIM_SOUTH) {
                   Monsters[Which].MonsterAnimator->UpdateAnimator(ANIM_SOUTH, true);
                    Monsters[Which].Facing=ANIM_SOUTH;
               }
          }
          else {
               if(Monsters[Which].Facing !=ANIM_NORTH) {
                   Monsters[Which].MonsterAnimator->UpdateAnimator(ANIM_NORTH, true);
                    Monsters[Which].Facing=ANIM_NORTH;
               }
          }
     }
}
