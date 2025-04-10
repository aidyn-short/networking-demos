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

    GameObjectRegistry::Get().Add(this);

}

void Player::HandleEvent(SDL_Event& event) 
{

    //If a key was pressed
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        //Adjust the velocity
        switch (event.key.keysym.sym)
        {
        case SDLK_UP: mVelY -= Player_VEL; break;
        case SDLK_DOWN: mVelY += Player_VEL; break;
        case SDLK_LEFT: mVelX -= Player_VEL; break;
        case SDLK_RIGHT: mVelX += Player_VEL; break;
        }
    }
    //If a key was released
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        //Adjust the velocity
        switch (event.key.keysym.sym)
        {
        case SDLK_UP: mVelY += Player_VEL; break;
        case SDLK_DOWN: mVelY -= Player_VEL; break;
        case SDLK_LEFT: mVelX += Player_VEL; break;
        case SDLK_RIGHT: mVelX -= Player_VEL; break;
        }
    }



}

void Player::Update(float deltaTime)
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

void Player::Render(SDL_Renderer* renderer, SDL_Point camPos)
{

    playerTexture.render(renderer, mPosX - camPos.x, mPosY - camPos.y);
}