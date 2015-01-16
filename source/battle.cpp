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

#include "battle.h"

BattleManager::BattleManager() {
     Options Opts;
     Verbosity=Opts.CombatVerbose;
}

BattleResult BattleManager::Battle(BattleInfo Attacker, BattleInfo Defender, int Type) {
     BattleResult Out;
     Out.AttackerDamage=0;
     Out.AttackerDamageType=0;
     Out.DefenderDamage=CalculateMeleeDamage(Attacker, Defender);
     Out.DefenderDamageType=DAMAGETYPE_PHYSICAL;

     return Out;
}

int BattleManager::CalculateMeleeDamage(BattleInfo Attacker, BattleInfo Defender) {
     double Miss=10, Hit=75, Evade=0, AttackPower;
     Evade=(100.0*(Defender.SkillEvasion+Defender.SkillEvasionMod)/(15.0*Defender.Level));
     if(Attacker.Level > Defender.Level) {
          if(Miss>0)
               Miss-=Attacker.Level-Defender.Level;
          else if(Evade>0)
               Evade-=Attacker.Level-Defender.Level;

     }
     else if(Attacker.Level < Defender.Level) {
          Miss+=Defender.Level-Attacker.Level;
          if(Hit>0)
               Hit-=Defender.Level-Attacker.Level;
     }
     if(Miss<0)     Miss=0;
     if(Hit<0)      Hit=0;
     if(Evade<0)    Evade=0;

     int TableResult=RandomNumber(0, 100);
     if(TableResult<Miss) {
          if(Verbosity == COMBAT_VERBOSE) MsgBox->AddMessage(MSGBOXRED+Attacker.Name+MSGBOXBLACK+" missed "+MSGBOXGREEN+Defender.Name+".");
          return 0;
     }
     if(TableResult<Miss+Evade) {
          if(Verbosity == COMBAT_VERBOSE) MsgBox->AddMessage(MSGBOXGREEN+Defender.Name+MSGBOXBLACK+" evaded "+MSGBOXRED+Attacker.Name+"'s strike.");
          return 0;
     }
     if(TableResult<Miss+Evade+Hit) {
          AttackPower=RandomNumber(Attacker.SkillStrength+Attacker.SkillStrengthMod, (Attacker.SkillStrength+Attacker.SkillStrengthMod)/2);
     }
     else {
          AttackPower=(RandomNumber(Attacker.SkillStrength+Attacker.SkillStrengthMod, (Attacker.SkillStrength+Attacker.SkillStrengthMod)/2)*1.5);
     }

     float DamageMitigation=(Defender.Armor/(100.0*Defender.Level));
     /* Return the difference between what the strike hit for, and what the defender absorbed */
     int DamageDone=(int)((AttackPower) * (1.0-DamageMitigation));
     if(DamageDone < 0) DamageDone=0;
     if(Verbosity > COMBAT_NORMAL) MsgBox->AddMessage(MSGBOXRED+Attacker.Name+MSGBOXBLACK+" hit "+MSGBOXGREEN+Defender.Name+MSGBOXBLACK+" for "+MSGBOXRED+IntToString(DamageDone)+" PHYSICAL "+MSGBOXBLACK+"damage.");
     return DamageDone;
}


