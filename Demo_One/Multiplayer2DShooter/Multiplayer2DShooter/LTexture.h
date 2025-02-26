#pragma once
#include <iostream>
#include <SDL3/sdl.h>
#include <SDL3_image/SDL_image.h>


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

    //Renders texture at given point
    void render(SDL_Renderer* renderer, int x, int y);

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

