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

#include "inventory.h"

ItemHandler * GlobalItemHandler;
ErrorHandler * Error;

/* Item Handler */
ItemHandler::ItemHandler() {
     if(!Error)
          Error=new ErrorHandler("Items", ERROR_SEVERITY_LOG, false);
}
ItemHandler::~ItemHandler() {
     delete Error;
}
bool ItemHandler::LoadItem(int ID) {
     int Index=0;
     /* Binary search our existing loaded items (if any)*/
     if(LoadedItems.size() > 0) {
          Index=ItemSearch(ID);
          if(Index!=-1)
               return true;
     }
     IlluvienXMLParser * Parser;
     Options * Opts=new Options;
     std::string Path=Opts->ModDirectory+"/data/items.xml";
     Parser=new IlluvienXMLParser(Path.c_str());
     if(!Parser->CheckFile()) {
          Error->ReportError(ERROR_SEVERITY_FATAL, "Could not load items.xml!");
     }
	bool Return=false;
     Parser->Parse();
     IlluvienXMLTag * Head=Parser->GetHead();
     for(unsigned int i=0; i<Head->Children.size(); i++) {
		for(unsigned int j=0; j<Head->Children[i]->Children.size(); j++) {
			if(Head->Children[i]->Children[j]->Name=="ID")
				if(StringToInt(Head->Children[i]->Children[j]->Data) == ID)
					ProcessTag(*Head->Children[i]);
					break;
					Return=true;
		}
	}
	delete Parser;
     delete Opts;
	if(Return==true)
		return true;
     Error->ReportError(ERROR_SEVERITY_FATAL, "Failed to Load item #"+IntToString(ID));
     return false;
}

Item ItemHandler::RetrieveItem(int ID) {
     int Index=0;
     /* Binary search our existing loaded items (if any)*/
     if(LoadedItems.size() > 0) {
          Index=ItemSearch(ID);
          if(Index!=-1)
               return LoadedItems[Index];
     }
     Error->ReportError(ERROR_SEVERITY_WARN, "Item #"+IntToString(ID)+" was not preloaded!");
     LoadItem(ID);
     return RetrieveItem(ID);
}

int ItemHandler::ItemSearch(int ID) {
     int l=0;
     int r=LoadedItems.size();
     int m=r/2;
     while(l!=r) {
          if(LoadedItems[m].ID == ID)
               return m;
          if(LoadedItems[m].ID > ID) {
               l=m;
               m=(r-l)/2;
          }
          else {
               r=m;
               m=(r/2);
          }
     }
     return -1;
}

void ItemHandler::ProcessTag(IlluvienXMLTag Tag) {
	Item Current;
	for(unsigned int i=0; i<Tag.Children.size(); i++) {

     	if(Tag.Children[i]->Name == "name") {
          	Current.Name=Tag.Children[i]->Data;
     	}
     	else if(Tag.Children[i]->Name == "ID") {
          	Current.ID=StringToInt(Tag.Children[i]->Data);
     	}
     	else if(Tag.Children[i]->Name == "maxstack") {
          	Current.MaxStack=StringToInt(Tag.Children[i]->Data);
     	}
     	else if(Tag.Children[i]->Name == "stack") {
          	Current.CurrentStack=StringToInt(Tag.Children[i]->Data);
     	}
     	else if(Tag.Children[i]->Name == "type") {
          	Current.Type=Tag.Children[i]->Data[0];
     	}
     	else if(Tag.Children[i]->Name == "icon") {
          	int Icon=StringToInt(Tag.Children[i]->Data);
			Current.Icon=LoadIcon(Icon);
     	}
     	else {
          	Error->ReportError(ERROR_SEVERITY_WARN, "Parsed Unknown Tag: "+Tag.Children[i]->Name+" with data: "+Tag.Children[i]->Data);
     	}
	}
	LoadedItems.push_back(Current);
}

SPRITE_REF * ItemHandler::LoadIcon(int ID) {
#ifdef UNIMPLEMENTED
	for(unsigned int i=0; i<IconCache.size(); i++) {
		if(IconCache[i]->ID==ID)
			IconCache[i]->refcount++;
			return IconCache[i];
	}
	Options * Opts=new Options;
	std::string Path=Opts->ModDirectory+"/images/maps/";
     Path+="items.bmp";
	BITMAP * Icons=load_bitmap(Path.c_str(),NULL);
	BITMAP * In=create_bitmap(32,32);
	blit(Icons, In, 0, 32*ID, 0,0,32,32);
	SPRITE_REF * Ref;
	Ref->ID=ID;
	Ref->refcount=1;
	Ref->sprite=get_rle_sprite(In);
	destroy_bitmap(In);
	destroy_bitmap(Icons);
	IconCache.push_back(Ref);
	delete Opts;
	return Ref;
#endif
    return NULL;
}

