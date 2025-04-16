#pragma once
#include <SDL2/SDL.h>
#include "Time.h"



class GameObject
{
public:

	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer, SDL_Point camPos) = 0;
	virtual void HandleEvent(SDL_Event& event) = 0;
	virtual bool HandleCollision(GameObject* collidingObject) = 0;

	bool GetEnabled() { return enabled; }

	bool enabled = true;

	SDL_FRect collision;
	Texture texture;
	float posX, posY;
	float angle;
};