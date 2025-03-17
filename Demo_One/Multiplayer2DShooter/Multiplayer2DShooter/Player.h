#pragma once
#include <SDL2/SDL.h>
#include "LTexture.h"


class Player
{
public:
    //The dimensions of the Player
    static const int Player_WIDTH = 20;
    static const int Player_HEIGHT = 20;

    //Maximum axis velocity of the Player
    static const int Player_VEL = 10;

    //Initializes the variables
    Player(LTexture& playerTexture);

    //Takes key presses and adjusts the Player's velocity
    void handleEvent(SDL_Event& e);

    //Moves the Player
    void move();

    //Shows the Player on the screen
    void render(SDL_Renderer* renderer, SDL_Point camPos);

    int getPosX();
    int getPosY();



private:
    //The X and Y offsets of the Player
    int mPosX, mPosY;

    //The velocity of the Player
    int mVelX, mVelY;

    LTexture playerTexture;
};