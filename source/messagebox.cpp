#include "messagebox.h"
#include "FontManager.h"

IlluvienMessageBox * MsgBox;

#define TEXT_HEIGHT 10

IlluvienMessageBox::IlluvienMessageBox() {
    Prerender = nullptr;
     Message NullMessage;
     TextPiece BlankPiece;
     BlankPiece.Color = { 0, 0, 0 };
     BlankPiece.Text=" ";
     NullMessage.Pieces.push_back(BlankPiece);
     for(int i=0; i<12; i++) {
          Messages.push_back(NullMessage);
     }
     SurfacePrerender = SDL_CreateRGBSurface(0, 768, 128, 32, 0, 0, 0, 0);
}

IlluvienMessageBox::~IlluvienMessageBox() {
     for(int i=0; i<12; i++) {
          Messages[i].Pieces.clear();
     }
     SDL_DestroyTexture(Prerender);
     SDL_FreeSurface(SurfacePrerender);
}

void IlluvienMessageBox::AddMessage(std::string Msg) {
     Message Out;
     TextPiece CurrentPiece;
     std::string Red, Green, Blue;
     CurrentPiece.Color = { 0, 0, 0 };
     for(unsigned int i=0; i<Msg.length(); i++) {
          if(Msg[i]=='^') {
               if(CurrentPiece.Text.length() > 0) {
                    Out.Pieces.push_back(CurrentPiece);
                    CurrentPiece.Text.clear();
               }
               Red.assign(Msg, i+1,3);
               Green.assign(Msg, i+4, 3);
               Blue.assign(Msg, i+7, 3);
               CurrentPiece.Color = { StringToInt(Red), StringToInt(Green), StringToInt(Blue) };
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
    //It's safe to pass NULL if we haven't prerendered anything yet
    SDL_DestroyTexture(Prerender);
    Prerender = nullptr;
    SDL_FillRect(SurfacePrerender, nullptr, SDL_MapRGB(SurfacePrerender->format, 128, 128, 128));
     for(int i=0; i<12; i++) {
          RenderString(SurfacePrerender, i);
     }
}
void IlluvienMessageBox::Render() {
    if (Prerender == nullptr) {
        Prerender = SDL_CreateTextureFromSurface(DefaultRenderer, SurfacePrerender);
    }
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 640;
    dest.w = 768;
    dest.h = 128;
    SDL_RenderCopy(DefaultRenderer, Prerender, nullptr, &dest);
}
void IlluvienMessageBox::RenderString(SDL_Surface * Buffer, int Index) {
     int CurrentPixel=0;
     SDL_Surface * temptext=nullptr;
     SDL_Rect dest;
     dest.x = 0;
     dest.y = Index*TEXT_HEIGHT;
     for(unsigned int i=0; i<Messages[Index].Pieces.size(); i++) {
         temptext = TTF_RenderText_Blended(FontManager::GetInterfaceFont(), Messages[Index].Pieces[i].Text.c_str(), Messages[Index].Pieces[i].Color);
         dest.w = temptext->w;
         dest.h = temptext->h;
         SDL_BlitSurface(temptext, nullptr, Buffer, &dest);
         dest.x+=temptext->w;
         SDL_FreeSurface(temptext);
     }

}
