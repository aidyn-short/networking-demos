#pragma once
#include <SDL2/SDL.h>
#include "Texture.h"
#include "GameObject.h"
#include "Weapon.h"




class Player : public GameObject
{
public:
    //The dimensions of the Player
    static const int Player_WIDTH = 20;
    static const int Player_HEIGHT = 20;


    //Maximum axis velocity of the Player
    static const int Player_VEL = 200;

    //Initializes the variables
    Player();

    //Takes key presses and adjusts the Player's velocity
    void HandleEvent(SDL_Event& event) override;

    //Moves the Player
    void Update(float deltaTime) override;

    //Shows the Player on the screen
    void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

    bool HandleCollision(GameObject* collidingObject) override;

    int getPosX();
    int getPosY();


    std::string Write() override;

    void Read(std::vector<std::string> info) override;





private:


    //The velocity of the Player
    float velX,velY;

    bool solidCollision = false;

    Weapon* playerWeapon;

    int health = 100;

    std::string currentSprite = "player";


};