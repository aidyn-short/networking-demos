#pragma once
#include <SDL2/SDL.h>
#include "GameObjectRegistry.h"





class SceneManager;

const int SCREEN_HEIGHT = 540*1.5;
const int SCREEN_WIDTH = 960*1.5;


const int LEVEL_WIDTH = 2500;
const int LEVEL_HEIGHT = 2500;

const std::vector<SDL_Point> playerSpawns = { {200,200}, {LEVEL_WIDTH - 200,LEVEL_HEIGHT - 200 }, {LEVEL_WIDTH - 200,200}, {200, LEVEL_HEIGHT - 200} };



class Scene {

public:
	virtual ~Scene() {}
	virtual void Init(SDL_Renderer* renderer, SceneManager* manager) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;
	virtual void HandleEvent(SDL_Event& event) = 0;


};