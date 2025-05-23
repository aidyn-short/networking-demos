#pragma once
#include <SDL2/SDL.h>
#include "Texture.h"
#include <SDL2/SDL_ttf.h>
#include <functional>

struct  vector4
{
    int r;
    int g;
    int b;
    int a;
};



//The mouse button
class Button
{
public:
    //Initializes internal variables
    Button(SDL_Renderer* renderer, std::string fontName, int fontSize, SDL_Color textColor, std::string text, std::function<void()> event = NULL);

    ~Button()
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    //Sets top left position
    void setPosition(int x, int y);

    //Handles mouse event
    void handleEvent(SDL_Event* e);

    //Shows button sprite
    void render(SDL_Renderer* renderer);


    void SetText(SDL_Renderer* renderer, std::string text);

    void SetDimensions(int width, int height) { this->width = width; this->height = height; }

    int getTextWidth() { return texture.getWidth(); }
    int getTextHeight() { return texture.getHeight(); }

    int getWidth() { return width; }
    int getHeight() { return height; }

    void backgroundColor(SDL_Color color) { this->color = color; }



private:
    //Top left position
    SDL_Point mPosition;

    std::function<void()> event;

    Texture texture;

    int width = 300;
    int height = 100;

    SDL_Color color = { 0,0,0,255 };
    SDL_Color tempColor = { 0,0,0,255 };

    TTF_Font* font;
    SDL_Color textColor;

};