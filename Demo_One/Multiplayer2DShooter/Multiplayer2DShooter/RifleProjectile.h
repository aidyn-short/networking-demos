#pragma once
#include "GameObject.h"

class RifleProjectile : public GameObject
{
public:
	RifleProjectile();
	RifleProjectile(float playerPosX, float playerPosY, float angle);


	bool HandleCollision(GameObject* collidingObject) override;
	void Update(float deltaTime) override;
	void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

	std::string Write() override;

	void Read(std::vector<std::string> info) override;


	int damage = 5;

private:


	int speed = 750;
	float velocityX, velocityY;
	


};



