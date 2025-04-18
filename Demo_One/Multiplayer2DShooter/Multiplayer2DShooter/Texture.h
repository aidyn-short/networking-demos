#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class Texture
{
public:
    //Initializes variables
    Texture();

    //Deallocates memory
    ~Texture();

    //Loads image at specified path
    bool loadFromFile(SDL_Renderer* renderer ,std::string path);

    bool loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor, TTF_Font* font);

    //Deallocates texture
    void free();


    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);



    //Renders texture at given point
    void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth() { return mWidth * scaleX; }
    int getHeight() { return mHeight * scaleY; }

    void SetScale(double x, double y) {
        scaleX = x; 
        scaleY = y; 

    }

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;

    double scaleX =1;
    double scaleY =1;



};

