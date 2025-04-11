#pragma once
#include <SDL2/SDL.h>


class Time
{
public:
	static Time& Get() {
		static Time instance;
		return instance;
	}


	void Update() {
		deltaTime = SDL_GetTicks() - lastFrameTime;

		lastFrameTime = SDL_GetTicks();

	}


	float GetDeltaTime() { return deltaTime/1000; };

	float deltaTime;
	float lastFrameTime = 0;

private:

};

