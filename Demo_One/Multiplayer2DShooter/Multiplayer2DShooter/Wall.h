#pragma once
#include <SDL2/sdl.h>
#include "Texture.h"
#include "GameObject.h"


class Wall : public GameObject
{
public:
	Wall(SDL_Renderer* renderer, int posX, int posY, float angle, std::string textureName);
	~Wall();

private:

	SDL_Rect collision;
	Texture texture;
	int posX, posY;
	float angle;



	// Inherited via GameObject
	void Update(float deltaTime) override;

	void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

	void HandleEvent(SDL_Event& event) override;

};




Wall::Wall(SDL_Renderer* renderer, int posX, int posY, float angle, std::string textureName)
{
	this->posX = posX;
	this->posY = posY;
	this->texture.loadFromFile(renderer, textureName);
	collision = { posX, posY, texture.getWidth(), texture.getHeight() };
	this->angle = angle;
}

Wall::~Wall()
{
}

void Wall::Update(float deltaTime)
{
}

void Wall::Render(SDL_Renderer* renderer, SDL_Point camPos)
{
	texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
}

void Wall::HandleEvent(SDL_Event& event)
{
}
