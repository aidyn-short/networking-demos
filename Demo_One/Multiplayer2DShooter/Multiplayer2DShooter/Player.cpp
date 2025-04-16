#include "Player.h"



Player::Player(Texture& playerTexture)
{
    //Initialize the offsets
    posX = 0;
    posY = 0;

    //Initialize the velocity
    velX = 0;
    velY = 0;

    this->texture = playerTexture;

    collision.w = playerTexture.getWidth();
    collision.h = playerTexture.getHeight();

    objectType = "Player";
    id = SDL_GetTicks();




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
        case SDLK_UP: velY -= Player_VEL; break;
        case SDLK_DOWN: velY += Player_VEL; break;
        case SDLK_LEFT: velX -= Player_VEL; break;
        case SDLK_RIGHT: velX += Player_VEL; break;
        }
    }
    //If a key was released
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        //Adjust the velocity
        switch (event.key.keysym.sym)
        {
        case SDLK_UP: velY += Player_VEL; break;
        case SDLK_DOWN: velY -= Player_VEL; break;
        case SDLK_LEFT: velX += Player_VEL; break;
        case SDLK_RIGHT: velX -= Player_VEL; break;
        }
    }

 

 

}

void Player::Update(float deltaTime)
{



    //Move the dot left or right


    posX += velX * Time::Get().GetDeltaTime();
    if ((posX < 0) || (posX + Player_WIDTH > 1280)) {
        posX -= velX * Time::Get().GetDeltaTime();
    }




    posY += velY * Time::Get().GetDeltaTime();
    if ((posY < 0) || (posY + Player_HEIGHT > 960)) {
        posY -= velY * Time::Get().GetDeltaTime();

    }



    collision.x = posX;
    collision.y = posY;



  


}



int Player::getPosX()
{
    return posX;
}

int Player::getPosY()
{
    return posY;
}

void Player::Render(SDL_Renderer* renderer, SDL_Point camPos)
{

    texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
}

bool Player::HandleCollision(GameObject* collidingObject)
{
    solidCollision = true;
    std::cout << "Whoa" << std::endl;

    bool colliding = solidCollision;
    if (colliding)
    {
        //Move back
        posX -= velX * Time::Get().GetDeltaTime();
        posY -= velY * Time::Get().GetDeltaTime();
        solidCollision = false;
    }




    return false;
}
