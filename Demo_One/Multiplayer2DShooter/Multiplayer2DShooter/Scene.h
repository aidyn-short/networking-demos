#pragma once
#include <SDL2/SDL.h>



const int SCREEN_HEIGHT = 540;
const int SCREEN_WIDTH = 960;


class Scene {

public:
	virtual ~Scene() {}
	virtual void Init(SDL_Renderer* renderer) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;
	virtual void HandleEvent(SDL_Event& event) = 0;
};