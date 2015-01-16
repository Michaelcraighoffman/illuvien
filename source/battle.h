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

//!  Battle.h
/*!
    Handles the combat system
*/

#ifndef   BATTLE_H
#define   BATTLE_H

#include <string>
#include "globals.h"
#include "options.h"
#include "messagebox.h"

#define   DAMAGE_SOURCE_MELEE      0
#define   DAMAGE_SOURCE_RANGED     1
#define   DAMAGE_SOURCE_MAGIC      2

#define   DAMAGETYPE_PHYSICAL      0    /* Red Damage */
#define   DAMAGETYPE_SHADOW        1    /* Black Damage */
#define   DAMAGETYPE_HOLY          2    /* Yellow Damage */
#define   DAMAGETYPE_FIRE          3    /* Orange Damage */
#define   DAMAGETYPE_WATER         4    /* Blue Damage */
#define   DAMAGETYPE_THREAT        5    /* Threat Damage */

//! Describes all the information about a combatant
struct BattleInfo {
     /*! The name of the combatant */
     std::string Name;
     /*! The level of the combatant */
     int Level;
     /*! Current amount of health */
     int Health;
     /*! Maximum Health */
     int MaxHealth;
     /*! Current amount of Mana */
     int Mana;
     /*! Maximum mana */
     int MaxMana;

     /*! Base intrinsic Strength */
     int SkillStrength;
     /*! Base intrinsic Perception */
     int SkillPerception;
     /*! Base intrinsic Attunement */
     int SkillAttunement;
     /*! Base intrinsic Evasion */
     int SkillEvasion;
     /*! Base intrinsic Absorbtion */
     int SkillAbsorbance;
     /*! Base intrinsic Stealth */
     int SkillStealth;

     /*! Added Strength from static sources (gear, talents, etc NOT Buffs) */
     int SkillStrengthMod;
     /*! Added Perception from static sources (gear, talents, etc NOT Buffs) */
     int SkillPerceptionMod;
     /*! Added Attunement from static sources (gear, talents, etc NOT Buffs) */     
     int SkillAttunementMod;
     /*! Added Evasion from static sources (gear, talents, etc NOT Buffs) */     
     int SkillEvasionMod;
     /*! Added Absorbance from static sources (gear, talents, etc NOT Buffs) */     
     int SkillAbsorbanceMod;
     /*! Added Stealth from static sources (gear, talents, etc NOT Buffs) */
     int SkillStealthMod;

     /*! Armor from static sources (gear, talents, etc NOT Buffs) */
     int Armor;

     /*! The maximum distance the combatant can attack with melee */
     int MeleeRange;
     /*! The maximum distance the combatant can attack with ranged */    
     int RangedRange;
     /*! The speed the combatant can attack with melee */
     int MeleeSpeed;
     /*! Number of turns until the combatant can melee again */
     int MeleeCooldown;

     //vector <Spell> Spells;
};

//! Structure defining all the results from a concluded combat action
struct BattleResult {
     /*! The amount of damage the defender has taken */
     int DefenderDamage;
     /*! The type of damage the defender has taken */      
     char DefenderDamageType;
     /*! The amount of damage the attacker has taken */
     int AttackerDamage;
     /*! The type of damage the attacker has taken */
     char AttackerDamageType;
};
//! Class to conduct battles within
class BattleManager {
     protected:
          /*! Combat text verbosity, stored locally from options */
          int Verbosity;
          /*! Internal function to calculate melee damage between two attackers */
          int CalculateMeleeDamage(BattleInfo Attacker, BattleInfo Defender);
     public:
          BattleManager();
          /*! Function called to evaluate a combat action between the two combatants */
          BattleResult Battle(BattleInfo Attacker, BattleInfo Defender, int Type);
};

#endif
