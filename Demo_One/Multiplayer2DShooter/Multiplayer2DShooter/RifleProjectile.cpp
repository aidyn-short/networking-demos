#include "RifleProjectile.h"
#include "Time.h"





RifleProjectile::RifleProjectile(float playerPosX, float playerPosY, float angle) {

	float rad = angle  * (3.1415 / 180);

	float offset = 80;

	posX = playerPosX + cos(rad) * offset;
	posY = playerPosY + sin(rad) * offset;

	velocityX = cos(rad) * speed;
	velocityY = sin(rad) * speed;



	this->texture = *AssetRegistry::Get().GetAsset<Texture>("rifleProjectile");
	objectType = "rifleProjectile";

	collision.w = this->texture.getWidth();
	collision.h = this->texture.getHeight();

	this->angle = angle;
}


std::string RifleProjectile::Write()
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
	return playerInfo;
}

void RifleProjectile::Read(std::vector<std::string> info)
{
	SetClient(std::stoi(info[1]));
	SetID(std::stoi(info[2]));
	posX = std::stof(info[3]);
	posY = std::stof(info[4]);
	angle = std::stof(info[5]);



	collision.x = posX;
	collision.y = posY;

}





void RifleProjectile::Update(float deltaTime)
{



	posX += velocityX * Time::Get().GetDeltaTime();
	posY += velocityY * Time::Get().GetDeltaTime();


	collision.x = posX;
	collision.y = posY;





}


void RifleProjectile::Render(SDL_Renderer* renderer, SDL_Point camPos)
{

	texture.render(renderer, posX - camPos.x, posY - camPos.y, NULL, angle);
}


bool RifleProjectile::HandleCollision(GameObject* collidingObject)
{

	if (collidingObject->objectType == "wall" || collidingObject->objectType == "player")
	{

		SetEnalbed(false);

	}

	return false;
}