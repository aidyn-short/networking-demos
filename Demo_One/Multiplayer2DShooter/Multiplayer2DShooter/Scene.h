#pragma once
#include <SDL2/SDL.h>




class Scene {

public:
	virtual ~Scene() {}
	virtual void Init() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render(SDL_Renderer* renderer) = 0;
	virtual void HandleEvent(SDL_Event& event) = 0;
};