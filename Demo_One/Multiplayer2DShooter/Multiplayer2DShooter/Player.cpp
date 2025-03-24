#include "Player.h"

Player::Player(Texture& playerTexture)
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    this->playerTexture = playerTexture;

}

void Player::handleEvent(SDL_Event& e)
{

    //If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY -= Player_VEL; break;
        case SDLK_DOWN: mVelY += Player_VEL; break;
        case SDLK_LEFT: mVelX -= Player_VEL; break;
        case SDLK_RIGHT: mVelX += Player_VEL; break;
        }
    }
    //If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        //Adjust the velocity
        switch (e.key.keysym.sym)
        {
        case SDLK_UP: mVelY += Player_VEL; break;
        case SDLK_DOWN: mVelY -= Player_VEL; break;
        case SDLK_LEFT: mVelX += Player_VEL; break;
        case SDLK_RIGHT: mVelX -= Player_VEL; break;
        }
    }



}

void Player::move()
{
    //Move the dot left or right
    mPosX += mVelX;

    //If the dot went too far to the left or right
    if ((mPosX < 0) || (mPosX + Player_WIDTH > 1280) )
    {
        //Move back
        mPosX -= mVelX;
    }

    mPosY += mVelY;

    //If the dot went too far up or down
    if ((mPosY < 0) || (mPosY + Player_HEIGHT > 960))
    {
        //Move back
        mPosY -= mVelY;
    }
}



int Player::getPosX()
{
    return mPosX;
}

int Player::getPosY()
{
    return mPosY;
}

void Player::render(SDL_Renderer* renderer, SDL_Point camPos)
{

    playerTexture.render(renderer, mPosX - camPos.x, mPosY - camPos.y);
}