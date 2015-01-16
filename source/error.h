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

#ifndef   ERROR_H
#define   ERROR_H
#include <allegro.h>
#include <string>
#include <fstream>

#define   ERROR_SEVERITY_LOG       0
#define   ERROR_SEVERITY_WARN      1
#define   ERROR_SEVERITY_FATAL     2

//! Error Handler
/*! Controls error reporting for a single module of the game */
class ErrorHandler {
     protected:
          /*! The module this instance is reporting for */
          std::string Module;
          /*! The minimum severity to write to the log */
          int MinSeverity;      
          /*! Whether or not Errors are mirrored to the ingame messagebox */   
          bool MessageBoxMirror;
     public:
          /*! Constructor */
          ErrorHandler(std::string ModuleName, int Severity, bool Mirror);
          /*! Reports an error, echoing it to the console, the logfile, and 
              (optionally) the ingame messagebox */ 
          void ReportError(int Severity, std::string Message);
};

#endif
