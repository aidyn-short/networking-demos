#include "Player.h"
#include "GameObjectRegistry.h"
#include "Scene.h"
#include "RifleProjectile.h"
Player::Player()
{
    //Initialize the offsets
    posX = 0;
    posY = 0;

    //Initialize the velocity
    velX = 0;
    velY = 0;

    this->texture = *AssetRegistry::Get().GetAsset<Texture>("player");

    collision.w = this->texture.getWidth();
    collision.h = this->texture.getHeight();

    objectType = "player";

    





}

void Player::HandleEvent(SDL_Event& event) 
{


    if (event.type == SDL_MOUSEBUTTONDOWN)
    {
        if (playerWeapon!= NULL)
        {
            playerWeapon->Fire(posX + collision.w/2, posY + collision.h/2, angle);
        }
    
    }




    //If a key was pressed
    if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        //Adjust the velocity
        switch (event.key.keysym.sym)
        {
        case SDLK_w: velY -= Player_VEL; break;
        case SDLK_s: velY += Player_VEL; break;
        case SDLK_a: velX -= Player_VEL; break;
        case SDLK_d: velX += Player_VEL; break;
        }
    }
    //If a key was released
    else if (event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        //Adjust the velocity
        switch (event.key.keysym.sym)
        {
        case SDLK_w: velY += Player_VEL; break;
        case SDLK_s: velY -= Player_VEL; break;
        case SDLK_a: velX += Player_VEL; break;
        case SDLK_d: velX -= Player_VEL; break;
        }
    }

    if (event.type == SDL_MOUSEMOTION) {
        float objX = posX + texture.getWidth() / 2.0f;
        float objY = posY + texture.getHeight() / 2.0f;

        SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

        camera.x = (posX + texture.getWidth() / 2) - SCREEN_WIDTH / 2;
        camera.y = (posY + texture.getHeight() / 2) - SCREEN_HEIGHT / 2;

        if (camera.x < 0)
        {
            camera.x = 0;
        }
        if (camera.y < 0)
        {
            camera.y = 0;
        }
        if (camera.x > LEVEL_WIDTH - camera.w)
        {
            camera.x = LEVEL_WIDTH - camera.w;
        }
        if (camera.y > LEVEL_HEIGHT - camera.h)
        {
            camera.y = LEVEL_HEIGHT - camera.h;
        }

        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);
  
        float deltaX = mouseX + camera.x - objX;
        float deltaY = mouseY + camera.y - objY;
        float angle = atan2(deltaY, deltaX) * 180.0 / M_PI; // radians to degrees

        this->angle = angle;

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

std::string Player::Write()
{
    std::string playerInfo;
    playerInfo += objectType;
    playerInfo += "=";
    playerInfo += std::to_string(client);
    playerInfo += "=";
    playerInfo += std::to_string(this->GetID());
    playerInfo += "=";
    playerInfo += std::to_string(posX);
    playerInfo += "=";
    playerInfo += std::to_string(posY);
    playerInfo += "=";
    playerInfo += std::to_string(angle);
    playerInfo += "=";
    playerInfo += (currentSprite);

    return playerInfo;
}

void Player::Read(std::vector<std::string> info)
{
    SetClient(std::stoi (info[1]));
    SetID(std::stoi(info[2]));
    posX = std::stof(info[3]);
    posY = std::stof(info[4]);
    angle = std::stof(info[5]);
   
    if (currentSprite != info[6])
    {
        currentSprite = info[6];
        this->texture = *AssetRegistry::Get().GetAsset<Texture>(currentSprite);

    }
   


    collision.x = posX;
    collision.y = posY;

}

void Player::Render(SDL_Renderer* renderer, SDL_Point camPos)
{

    texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
}

bool Player::HandleCollision(GameObject* collidingObject)
{
    

    if (collidingObject->objectType == "rifle")
    {
        Weapon* weapon = dynamic_cast<Weapon*>(collidingObject);


        if (weapon)
        {
            
            weapon->held = true;
            weapon->visible = false;
            weapon->SetEnalbed(false);
        
      
           
            if (client == GameObjectRegistry::Get().GetClientNumber())
            {
                playerWeapon = weapon;
                std::cout << "Picked up weapon" << std::endl;
                currentSprite = "playerRifle";
                this->texture = *AssetRegistry::Get().GetAsset<Texture>("playerRifle");

            }

        }


        


    }








    if (client == GameObjectRegistry::Get().GetClientNumber())
    {
        if (collidingObject->objectType == "player" || collidingObject->objectType == "wall")
        {
            //Move back
            posX -= velX * Time::Get().GetDeltaTime();
            posY -= velY * Time::Get().GetDeltaTime();
            solidCollision = false;

        }
      
        if (collidingObject->objectType == "rifleProjectile")
        {
            
            RifleProjectile* projectile = dynamic_cast<RifleProjectile*>(collidingObject);

            if (projectile)
            {
                health -= projectile->damage;

                std::cout << "health at:  " << health << std::endl;

                if (health <= 0)
                {
                    posX = 0;
                    posY = 0;
                    health = 100;
                }


            }


        }


    }






    return false;
}
