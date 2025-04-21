#pragma once
#include "GameObject.h"


class Weapon : public GameObject
{
public:

	virtual void Fire(float playerPosX, float playerPosY, float angle) = 0;



	bool visible = true, held = false;





private:

};

