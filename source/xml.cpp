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

//TODO Remove custom XML implementation
#include "xml.h"

IlluvienXMLTag::IlluvienXMLTag() {
    Parent = nullptr;
}
IlluvienXMLTag::~IlluvienXMLTag() {
     while(Children.size()>0) {
          delete Children[0];
          Children.erase(Children.begin());
     }
}

IlluvienXMLParser::IlluvienXMLParser(std::string Filename) {
     Error=new ErrorHandler("XMLParser", ERROR_SEVERITY_LOG, false);
	File.open(Filename.c_str());
	if(!File.is_open()) { 	//File is NOT open
		std::string Err;
		Err+="Could not open file: ";
		Err+=Filename;
		Error->ReportError(ERROR_SEVERITY_WARN, Err.c_str());
	}
}
IlluvienXMLParser::~IlluvienXMLParser() {
     File.close();
     delete Error;
}
bool IlluvienXMLParser::CheckFile() {
     return File.is_open();
}
int IlluvienXMLParser::Parse() {
     if(File.is_open()==false) {
		Error->ReportError(ERROR_SEVERITY_FATAL, "Attempted to read from a closed file");
		return XML_FILENOTFOUND;
	}
     std::string Hold, In;
     unsigned int Pos=0;
     /* Copy the entire file into a string */
     while(getline(File, Hold))
          In+=Hold;
     /* Skip leading characters */
     while(Pos<In.size() && In[Pos] != '<')
          Pos++;
     Pos++;    /* Advance Pos past the opening bracket */
     Hold.clear();
     /* Read the head tag */
     while(Pos<In.size() && In[Pos] != '>'){
          Hold+=In[Pos];
          Pos++;
     }
     Pos++;    /* Advance Pos past the closing bracket */
     Head.Name=Hold;
     /* While loop to read all further tags and build tree */
     IlluvienXMLTag * CurrentTag=&Head;
     while(Pos<In.size()) {
          if(In[Pos] == ' ')
               Pos++;
          else if(In[Pos] == '<') {
               Pos++;
               if(In[Pos] == '/') {
                    Pos++;
                    Hold.clear();
                    while(In[Pos] != '>') {
                         Hold+=In[Pos];
                         Pos++;
                    }
                    Pos++;
                    if(Hold != CurrentTag->Name) {
                         Error->ReportError(ERROR_SEVERITY_WARN, "XML tag '"+CurrentTag->Name+"' closed out of order");
                         return XML_INCOMPLETETAG;
                    }
                    else {
                         CurrentTag=CurrentTag->Parent;
                         if (CurrentTag == nullptr) {
                              return XML_GOODTREE;
                         }
                    }
               }
               else {
                    IlluvienXMLTag * New=new IlluvienXMLTag;
                    CurrentTag->Children.push_back(New);
                    New->Parent=CurrentTag;
                    CurrentTag=New;
                    Hold.clear();
                    while(In[Pos] != '>') {
                         Hold+=In[Pos];
                         Pos++;
                    }
                    Pos++;
                    CurrentTag->Name=Hold;
               }
          }
          else {
               Hold.clear();
               while(In[Pos] != '<') {
                         Hold+=In[Pos];
                         Pos++;
               }
               CurrentTag->Data=Hold;  
          }
     }
     return XML_ENDOFFILE;
}

IlluvienXMLTag * IlluvienXMLParser::GetHead() {
     return &Head;
}
