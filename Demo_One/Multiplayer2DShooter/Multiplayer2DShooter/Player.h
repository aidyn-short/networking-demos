#pragma once
#include <SDL2/SDL.h>
#include "Texture.h"
#include "GameObject.h"
#include "GameObjectRegistry.h"

class Player : public GameObject
{
public:
    //The dimensions of the Player
    static const int Player_WIDTH = 20;
    static const int Player_HEIGHT = 20;

    //Maximum axis velocity of the Player
    static const int Player_VEL = 100;

    //Initializes the variables
    Player(Texture& playerTexture);

    //Takes key presses and adjusts the Player's velocity
    void HandleEvent(SDL_Event& event) override;

    //Moves the Player
    void Update(float deltaTime) override;

    //Shows the Player on the screen
    void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

    bool HandleCollision(GameObject* collidingObject) override;

    int getPosX();
    int getPosY();



private:


    //The velocity of the Player
    float velX,velY;

    bool solidCollision = false;

};