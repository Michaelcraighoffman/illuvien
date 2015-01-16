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

#ifndef   ILLUVIEN_XML_H
#define   ILLUVIEN_XML_H

#include <fstream>
#include <string>
#include <vector>

#include "error.h"

#define   XML_UNKNOWNERROR    0 /* Unclassified error */
#define   XML_FILENOTFOUND    1 /* The file could not be read from */
#define   XML_INCOMPLETETAG   2 /* The file contained an incomplete tag */
#define   XML_ENDOFFILE       3 /* The file ended before all tags were closed */
#define   XML_GOODTREE        4 /* No error */

//! A node of the tree constituting an entire XML document
class IlluvienXMLTag {
     public:
     //! The 'name' of this tag
     /*! IE, the text between the angle brackets < > */
     std::string Name;
     /*! The parent node of this tag */
     IlluvienXMLTag * Parent;
     /*! A vector containing all this tags child nodes */
     std::vector <IlluvienXMLTag *> Children;
     /*! If this node contains no children, a string containing
          the data between the start and end tags */
     std::string Data;

     IlluvienXMLTag();
     ~IlluvienXMLTag();
};

//! Parses and generates a Tree of nodes from a given .xml file
class IlluvienXMLParser {
     protected:
          /*! File we are reading from */
          std::ifstream File;
          /*! The Root of the tree*/
          IlluvienXMLTag Head;
          /*! Errorhandler */   
          ErrorHandler * Error;
     public:
          IlluvienXMLParser(std::string Filename);
		~IlluvienXMLParser();
          /*! Verifies that the file was opened successfully */
		bool CheckFile();
          /*! Parses the file and creates the tree, return XML_GOODTREE on success*/
		int Parse();
          /*! Returns the root node */
          IlluvienXMLTag * GetHead();
};

#endif


