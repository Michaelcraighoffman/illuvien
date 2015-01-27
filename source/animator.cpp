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

#include <SDL_image.h>
#include <SDL_ttf.h>

#include "animator.h"
#include "fontmanager.h"

Animator::~Animator() {
    for (Animation a : Animations) {
        for (SDL_Texture * t : a.Frames) {
            SDL_DestroyTexture(t);
        }
    }

}

bool Animator::operator<(const Animator &B) {
    /* Once Animators > Simple Animators > full Animators */
    if (this->Type < B.Type)
        return false;
    if (this->Type > B.Type)
        return true;

    if (this->Position.y != B.Position.y)
        return this->Position.y < B.Position.y;
    /* The y-values are equal */
    if (this->Position.x != B.Position.x)
        return this->Position.x < B.Position.x;

    /* The Animators are on the same tile */


    return false;
}

void Animator::LoadImages(std::string Path) {
    Options * Opts = new Options;
    SDL_Surface * Image;
    SDL_Surface * Data = IMG_Load(Path.c_str());
    if (!Data) {
        GlobalError->ReportError(ERROR_SEVERITY_FATAL, "Could not load image: " + Path);
        return;
    }
    SDL_Rect src;
    src.w = Data->w / 4;
    src.h = Data->h / NUM_ANIMATIONS;
    Image = SDL_CreateRGBSurface(0, Data->w / 4, Data->h / NUM_ANIMATIONS, 32, 0, 0, 0, 0);
    SDL_SetColorKey(Image, SDL_TRUE, SDL_MapRGB(Image->format, 255, 0, 255));
    for (int i = 0; i<NUM_ANIMATIONS; i++) {

        if (!Image) {
            GlobalError->ReportError(ERROR_SEVERITY_FATAL, "Could not create Bitmap");
            return;
        }
        src.x = 0;
        src.y = i*(Data->h / NUM_ANIMATIONS);
        SDL_BlitSurface(Data, &src, Image, nullptr);
        Animations[i].Frames[0] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        src.x += (Data->w / 4);
        SDL_BlitSurface(Data, &src, Image, nullptr);
        Animations[i].Frames[1] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[2] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[3] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        src.x += (Data->w / 4);
        SDL_BlitSurface(Data, &src, Image, nullptr);
        Animations[i].Frames[4] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[5] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[6] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        src.x += (Data->w / 4);
        SDL_BlitSurface(Data, &src, Image, nullptr);
        Animations[i].Frames[7] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[8] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
        Animations[i].Frames[9] = SDL_CreateTextureFromSurface(DefaultRenderer, Image);
    }
    SDL_FreeSurface(Image);
    SDL_FreeSurface(Data);
    delete Opts;
}


void Animator::Create(int animType, int animDuration, std::string Path, Point Location) {
    Type = animType;
    Action = ANIM_IDLE;
    LoadImages(Path);
    Position = Location * 32;
    Destination = Location * 32;
    CurrentFrame = 0;
    FrameTime = 0;
    LastMovement = 0;
    Duration = animDuration;
    AnimatorFinished = false;
}

void Animator::CreateDamageAnimator(int Amount, Point Location) {
    SDL_Surface * Hold;
    if (Amount > 0)
        Hold = TTF_RenderText_Solid(FontManager::GetOverlayFont(), IntToString(Amount).c_str(), { 255, 0, 0 });
    else
        Hold = TTF_RenderText_Solid(FontManager::GetOverlayFont(), IntToString(Amount).c_str(), { 0, 255, 0 });
    SDL_Texture * final = SDL_CreateTextureFromSurface(DefaultRenderer, Hold);
    for (int i = 0; i < NUM_ANIMATIONS; i++) {
        for (int j = 0; j < 10; j++) {
            Animations[i].Frames[j] = final;
        }
    }
    Type = ANIM_ONCE;
    Action = ANIM_IDLE;
    Position = Location*32;
    Destination = Point(Position.x, Position.y - 32);
    CurrentFrame = 0;
    FrameTime = 0;
    LastMovement = 0;
    Duration = 500;
    AnimatorFinished = false;
    SDL_FreeSurface(Hold);
}

void Animator::UpdateAnimator(int newAction, bool makeIdle) {
    UpdateAnimator(newAction, Destination / 32, makeIdle);
}
void Animator::UpdateAnimator(Point newDestination, bool makeIdle) {
    UpdateAnimator(Action, newDestination, makeIdle);
}
void Animator::UpdateAnimator(int newAction, Point newDestination, bool makeIdle) {

    Action = newAction;
    Destination.x = newDestination.x * 32;
    Destination.y = newDestination.y * 32;
    Idle = false;
    CurrentFrame = 0;
    FrameTime = 0;
    LastMovement = 0;
    if (makeIdle == true) {
        Idle = true;
    }
}

Point Animator::GetPosition() {
    return Position;
}

void Animator::MoveTowardsDestination() {
    int PixelTime = ((Duration / 10) / 3);
    while (LastMovement >= PixelTime) {
        LastMovement -= PixelTime;
        if (Destination.x > (Position.x)) {
            Position.x += 1;
            if (Destination.x < (Position.x))
                Position.x = Destination.x;
        }
        else if (Destination.x < (Position.x)) {
            Position.x -= 1;
            if (Destination.x >(Position.x))
                Position.x = Destination.x;
        }
        if (Destination.y >(Position.y)) {
            Position.y += 1;
            if (Destination.y < (Position.y))
                Position.y = Destination.y;
        }
        else if (Destination.y < (Position.y)) {
            Position.y -= 1;
            if (Destination.y >(Position.y))
                Position.y = Destination.y;
        }
    }
}

void Animator::Update(int delta) {
    FrameTime += delta;
    LastMovement += delta;
    MoveTowardsDestination();
    CurrentFrame = (FrameTime / (Duration / 10));
    if (CurrentFrame > 9) {
        FrameTime -= Duration;
        CurrentFrame = 0;
        if (Type == ANIM_FULL) {
            Idle = true;
            Position = Destination;
        }
        else if (Type == ANIM_ONCE) {
            AnimatorFinished = true;
        }
    }
}

void Animator::Render(Point Actual) {
    int Offsetx, Offsety, Width, Height;
    SDL_Rect dest;
    if (Position.x >= (Actual.x - 64)
        && Position.x < (Actual.x + 768 + 64)
        && Position.y >= (Actual.y - 64)
        && Position.y < (Actual.y + 640 + 64)){
        SDL_QueryTexture(Animations[0].Frames[0], nullptr, nullptr, &Width, &Height);
        Offsetx = Width - 32;
        Offsety = Height - 32;
        dest.x = ((Position.x - (Actual.x)) - Offsetx);
        dest.y = ((Position.y - (Actual.y)) - Offsety);
        dest.w = Width;
        dest.h = Height;
        if (Idle == true)
            SDL_RenderCopy(DefaultRenderer, Animations[Action].Frames[0], nullptr, &dest);
        else
            SDL_RenderCopy(DefaultRenderer, Animations[Action].Frames[CurrentFrame], nullptr, &dest);
    }
}
void Animator::FinishAnimation() {
    AnimatorFinished = true;
}
bool Animator::IsFinished() {
    return AnimatorFinished;
}

