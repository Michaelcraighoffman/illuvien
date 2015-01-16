
#include "messagebox.h"

IlluvienMessageBox * MsgBox;

IlluvienMessageBox::IlluvienMessageBox() {
     Message NullMessage;
     TextPiece BlankPiece;
     BlankPiece.Color=makecol(0,0,0);
     BlankPiece.Text=" ";
     NullMessage.Pieces.push_back(BlankPiece);
     for(int i=0; i<12; i++) {
          Messages.push_back(NullMessage);
     }
     Prerender=create_bitmap(768, 128);
}

IlluvienMessageBox::~IlluvienMessageBox() {
     for(int i=0; i<12; i++) {
          Messages[i].Pieces.clear();
     }
}

void IlluvienMessageBox::AddMessage(std::string Msg) {
     Message Out;
     TextPiece CurrentPiece;
     std::string Red, Green, Blue;
     CurrentPiece.Color=makecol(0,0,0);
     for(unsigned int i=0; i<Msg.length(); i++) {
          if(Msg[i]=='^') {
               if(CurrentPiece.Text.length() > 0) {
                    Out.Pieces.push_back(CurrentPiece);
                    CurrentPiece.Text.clear();
               }
               Red.assign(Msg, i+1,3);
               Green.assign(Msg, i+4, 3);
               Blue.assign(Msg, i+7, 3);
               CurrentPiece.Color=makecol(StringToInt(Red),StringToInt(Green),StringToInt(Blue));
               i+=9;
          }
          else {
               CurrentPiece.Text+=Msg[i];
          }
     }
     if(CurrentPiece.Text.length() > 0) {
          Out.Pieces.push_back(CurrentPiece);
     }
     AddMessage(Out);
}

void IlluvienMessageBox::AddMessage(Message Msg) {
     Messages.push_back(Msg);
     Messages.erase(Messages.begin());
     ReRender();
}
void IlluvienMessageBox::ReRender() {
     rectfill(Prerender, 0,0,768, 128, makecol(128,128,128));
     for(int i=0; i<12; i++) {
          RenderString(Prerender, i);
     }
}
void IlluvienMessageBox::Render(BITMAP * Buffer) {
     blit(Prerender, Buffer, 0,0,0,640, 768, 128);
}
void IlluvienMessageBox::RenderString(BITMAP * Buffer, int Which) {
     int CurrentPixel=0;
     for(unsigned int i=0; i<Messages[Which].Pieces.size(); i++) {
          textprintf_ex(Buffer, font, CurrentPixel,(Which*10), Messages[Which].Pieces[i].Color, -1, "%s", Messages[Which].Pieces[i].Text.c_str());
          CurrentPixel+=Messages[Which].Pieces[i].Text.length()*8;
     }
}
