#pragma once
#include "Weapon.h"


class Rifle : public Weapon 
{
public:
	Rifle(float posX, float posY);
	~Rifle();

private:



	void Render(SDL_Renderer* renderer, SDL_Point camPos) override;

	void Fire(float playerPosX, float playerPosY, float angle)  override;


	bool HandleCollision(GameObject* collidingObject) override;



};
