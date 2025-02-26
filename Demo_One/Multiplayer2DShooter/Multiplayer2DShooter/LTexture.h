#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>


class LTexture
{
public:
    //Initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(SDL_Renderer* renderer ,std::string path);

    //Deallocates texture
    void free();


    void SetColor(Uint8 red, Uint8 green, Uint8 blue);

    void setBlendMode(SDL_BlendMode blending);

    void setAlpha(Uint8 alpha);



    //Renders texture at given point
    void render(SDL_Renderer* renderer, int x, int y, double angle = 0.0, SDL_Point*  center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Rect* clip = NULL);

    //Gets image dimensions
    int getWidth() { return mWidth; }
    int getHeight() { return mHeight; }

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;



};

