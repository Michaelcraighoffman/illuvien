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

#ifndef   INVENTORY_H
#define   INVENTORY_H

#include <SDL.h>
#include <string>
#include <vector>

#include "error.h"
#include "globals.h"
#include "xml.h"

struct Item {
     int ID;             /* Unique ID of the item */
     std::string Name;   /* The name of the item */
     int MaxStack;       /* The maximum number of this item that can be present
                            in one stack */
     int CurrentStack;   /* The number of items in this stack */
     char Type;          /* What type of item this is */
     SPRITE_REF * Icon;           /* ID of the icon for the item */
};

class ItemHandler {
    protected:
          std::vector <Item> LoadedItems;
          std::vector <SPRITE_REF *> IconCache;
    public:
          ItemHandler();
          ~ItemHandler();
          bool LoadItem(int ID);
		SPRITE_REF * LoadIcon(int ID);
          Item RetrieveItem(int ID);
          int ItemSearch(int ID); 
          void ProcessTag(IlluvienXMLTag Tag);
		  void Render();
};
    

class Inventory {
     protected:
          Item Slots[30];
          Item Equipment[6];
     public:
          bool AddItem(Item In);
          bool TakeItem(Item In);
          int CountItem(Item In);
		void Render();
};
extern ItemHandler * GlobalItemHandler;
extern ErrorHandler * Error;
#endif
