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

#include "error.h"
#include "globals.h"

ErrorHandler::ErrorHandler(std::string ModuleName, int Severity, bool Mirror) {
     Module=ModuleName;
     MinSeverity=Severity;
     MessageBoxMirror=Mirror;
}
void ErrorHandler::ReportError(int Severity, std::string Message) {
     std::string e;
     if(Severity >= MinSeverity) {
          e=Module+":";
          switch(Severity) {
               case ERROR_SEVERITY_LOG:
                    e+="LOG:";
                    break;
               case ERROR_SEVERITY_WARN:
                    e+="WARN:";
                    break;
               case ERROR_SEVERITY_FATAL:
                    e+="FATAL:";
                    break;
               default:
                    e+="UNKNOWN:";
                    break;
          }
          e+=Message;
          std::ofstream File("error.txt", std::ios::app);
          File<<e;
          File<<"\n";
          File.close();
          if(MessageBoxMirror==true || Severity==ERROR_SEVERITY_FATAL) {
               PopupBox(e);
          }
          if(Severity==ERROR_SEVERITY_FATAL) {
               exit(255);
          }
     }
}
