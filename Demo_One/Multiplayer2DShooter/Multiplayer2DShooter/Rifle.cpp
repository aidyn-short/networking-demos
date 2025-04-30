#include "Rifle.h"
#include "AssetRegistry.h"
#include "GameObjectRegistry.h"
#include "RifleProjectile.h"



Rifle::Rifle(float posX, float posY)
{

	this->texture = *AssetRegistry::Get().GetAsset<Texture>("rifle");


	this->posX = posX;
	this->posY = posY;
	this->collision.x = posX;
	this->collision.y = posY;

	collision.w = this->texture.getWidth();
	collision.h = this->texture.getHeight();
	objectType = "rifle";
	GameObjectRegistry::Get().Add(this);




}

Rifle::~Rifle()
{
}


void Rifle::Render(SDL_Renderer* renderer, SDL_Point camPos)
{

	if (visible)
	{
		texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
		
	}
}



void Rifle::Fire(float playerPosX, float playerPosY, float angle) {

	RifleProjectile* projectile = new RifleProjectile(playerPosX, playerPosY, angle);
	projectile->SetClient(GameObjectRegistry::Get().GetClientNumber());
	GameObjectRegistry::Get().Add(projectile);


}

bool Rifle::HandleCollision(GameObject* collidingObject)
{

	if (collidingObject->objectType == "wall")
	{

		SetEnalbed(false);

	}


	return false;
}



