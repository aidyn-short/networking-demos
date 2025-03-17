#pragma once
#include <SDL2/SDL.h>
#include "LTexture.h"
#include <SDL2/SDL_ttf.h>

struct  vector4
{
    int r;
    int g;
    int b;
    int a;
};



//The mouse button
class LButton
{
public:
    //Initializes internal variables
    LButton(SDL_Renderer* renderer, std::string fontName, int fontSize, SDL_Color textColor, std::string text);

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    void handleEvent(SDL_Event* e);

    //Shows button sprite
    void render(SDL_Renderer* renderer);


    void SetText(SDL_Renderer* renderer, std::string text);

private:
    //Top left position
    SDL_Point mPosition;

    
    LTexture texture;

    int width = 300;
    int height = 100;

    vector4 color = { 255,0,0,255 };

    TTF_Font* font;
    SDL_Color textColor;

};