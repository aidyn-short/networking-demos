#pragma once
#include <SDL2/sdl.h>
#include "Texture.h"
#include "GameObject.h"
#include "GameObjectRegistry.h"
#include "AssetRegistry.h"

class Wall : public GameObject
{
public:
	Wall(SDL_Renderer* renderer, float posX, float posY, float angle, std::string textureName);
	~Wall();


	void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

	// Inherited via GameObject
	void Update(float deltaTime) override;

	void HandleEvent(SDL_Event& event) override;

	bool HandleCollision(GameObject* collidingObject) override;
private:






	

};




Wall::Wall(SDL_Renderer* renderer, float posX, float posY, float angle, std::string textureName)
{
	this->posX = posX;
	this->posY = posY;
	this->texture = *AssetRegistry::Get().GetAsset<Texture>("wall");
	collision = { posX, posY, (float)texture.getWidth(), (float)texture.getHeight() };
	this->angle = angle;
	this->objectType = "wall";
	GameObjectRegistry::Get().Add(this);

}

Wall::~Wall()
{
}


void Wall::Render(SDL_Renderer* renderer, SDL_Point camPos)
{
	texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
}

void Wall::Update(float deltaTime)
{
}

void Wall::HandleEvent(SDL_Event& event)
{
}

bool Wall::HandleCollision(GameObject* collidingObject)
{




	return false;
}




